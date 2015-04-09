// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>

#include "Vec3.h"
#include "Camera.h"
#include "Mesh.h"
#include "GLProgram.h"

using namespace std;

static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static const string DEFAULT_MESH_FILE ("models/man.off");

static string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Shaders");
static GLint window;
static unsigned int FPS = 0;
static bool fullScreen = false;

static Camera camera;
static Mesh mesh;
static float roughness = 0.3;
Program * glProgram;

void printUsage () {
  std::cerr << std::endl 
	    << appTitle << std::endl
	    << "Author: Tamy Boubekeur" << std::endl << std::endl
	    << "Usage: ./main [<file.off>]" << std::endl
	    << "Commands:" << std::endl 
	    << "------------------" << std::endl
	    << " ?: Print help" << std::endl
	    << " w: Toggle wireframe mode" << std::endl
	    << " <drag>+<left button>: rotate model" << std::endl 
	    << " <drag>+<right button>: move model" << std::endl
	    << " <drag>+<middle button>: zoom" << std::endl
	    << " q, <esc>: Quit" << std::endl << std::endl; 
}

void init (const char * modelFilename) {
  glewInit();
  glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
  glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
  glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
  glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
  glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
	
  camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT); // Setup the camera
  mesh.loadOFF (modelFilename); // Load a mesh file
  try {
    glProgram = Program::genVFProgram ("Simple GL Program", "shader.vert", "shader.frag"); // Load and compile pair of shaders
    glProgram->use (); // Activate the shader program
     //va falloir trouver la valeur adéquat du program object
    glProgram->Program::setUniform1f("roughness_shader", roughness);
  } catch (Exception & e) {
    cerr << e.msg () << endl;
  }
}

void drawScene () {

  glBegin (GL_TRIANGLES);
  for (unsigned int i = 0; i < mesh.T.size (); i++) 
    for (unsigned int j = 0; j < 3; j++) {
      const Vertex & v = mesh.V[mesh.T[i].v[j]];
      glNormal3f (v.n[0], v.n[1], v.n[2]); // Specifies current normal vertex   
      glVertex3f (v.p[0], v.p[1], v.p[2]); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)
    }
  glEnd (); 
}

void reshape(int w, int h) {
  camera.resize (w, h);
}

void display () {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera.apply (); 
  drawScene ();
  glFlush ();
  glutSwapBuffers (); 
}

void key (unsigned char keyPressed, int x, int y) {
  switch (keyPressed) {
  case 'f':
    if (fullScreen) {
      glutReshapeWindow (camera.getScreenWidth (), camera.getScreenHeight ());
      fullScreen = false;
    } else {
      glutFullScreen ();
      fullScreen = true;
    }      
    break;
  case 'q':
  case 27:
      exit (0);
      break;
  case '+' :
    {
    roughness  = -max(-1.0, -roughness-0.05);
    std::cout << roughness;
    glProgram->Program::setUniform1f("roughness_shader", roughness);
    break;
    }

  case '-' : 
  {
    roughness  = max(0.0, roughness-0.05);
    //Program::setUniform1f(maVariable,roughness);
    break;
  }
  case 'w':
    GLint mode[2];
    glGetIntegerv (GL_POLYGON_MODE, mode);
    glPolygonMode (GL_FRONT_AND_BACK, mode[1] ==  GL_FILL ? GL_LINE : GL_FILL);
    break;
    break;
  default:
    printUsage ();
    break;
  }
}

void mouse (int button, int state, int x, int y) {
  camera.handleMouseClickEvent (button, state, x, y);
}

void motion (int x, int y) {
  camera.handleMouseMoveEvent (x, y);
}

void idle () {
  static float lastTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
  static unsigned int counter = 0;
  counter++;
  float currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
  if (currentTime - lastTime >= 1000.0f) {
    FPS = counter;
    counter = 0;
    static char winTitle [128];
    unsigned int numOfTriangles = mesh.T.size ();
    sprintf (winTitle, "Number Of Triangles: %d - FPS: %d", numOfTriangles, FPS);
    glutSetWindowTitle (winTitle);
    lastTime = currentTime;
  }
  glutPostRedisplay (); 
}

int main (int argc, char ** argv) {
  if (argc > 2) {
    printUsage ();
    exit (1);
  }
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
  window = glutCreateWindow (appTitle.c_str ());
  init (argc == 2 ? argv[1] : DEFAULT_MESH_FILE.c_str ());
  glutIdleFunc (idle);
  glutReshapeFunc (reshape);
  glutDisplayFunc (display);
  glutKeyboardFunc (key);
  glutMotionFunc (motion);
  glutMouseFunc (mouse);
  printUsage ();  
  glutMainLoop ();
  return 0;
}

