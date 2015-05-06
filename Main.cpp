// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------
#include <GL/glew.h>
// #include <GL/gl.h>
#include <GL/freeglut.h>
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
#include "render.h"


#include <pthread.h>
void junk() {
int i;
i = pthread_getconcurrency();
};

using namespace std;

static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static const string DEFAULT_MESH_FILE ("material/models/man.off");

static string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Shaders");
static GLint window;
static unsigned int FPS = 0;
static bool fullScreen = false;
static bool rotateLight = false;
//static float rotateLightAngle = 0.0;
static Camera camera;
static Mesh mesh;
Render * renderTime;

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



void reshape(int w, int h) {
  camera.resize (w, h);
}

void display () {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //  Je comprends pas pourquoi ca clignote ! Maintenant si !
  camera.apply ();
  renderTime->drawScene ();
  glFlush ();
  glutSwapBuffers (); 
}

void key (unsigned char keyPressed, int x, int y)
{
  switch (keyPressed)
  {
    case 'f':
    if (fullScreen)
    {
      glutReshapeWindow (camera.getScreenWidth (), camera.getScreenHeight ());
      fullScreen = false;
    } 
    else {
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
    GLfloat panams;
    renderTime->pRInfo.secondPass->Program::getUniform("roughness_shader", &panams);
    panams  = -max(-1.0, -panams-0.05);
    renderTime->pRInfo.secondPass->Program::setUniform1f("roughness_shader", panams);
    std::cout << "roughness : " <<  panams <<std::endl;
    break;
  }

  case '-' : 
  {
    GLfloat panams;
    renderTime->pRInfo.secondPass->Program::getUniform("roughness_shader", &panams);
    panams  = max(0.0, panams-0.05);
    std::cout << panams;
    renderTime->pRInfo.secondPass->Program::setUniform1f("roughness_shader", panams);
    std::cout << "roughness : " <<  panams <<std::endl;
    break;
  }
  
  case 'a' : 
  {
    GLfloat panams;
    renderTime->pRInfo.secondPass->Program::getUniform("coeffFresnel", &panams);
    panams  = max(0.0, panams-0.05);
    std::cout << panams;
    renderTime->pRInfo.secondPass->Program::setUniform1f("coeffFresnel", panams);
    std::cout << "coeffFresnel : " <<  panams <<std::endl;
    break;
  }

  case 'z' :
  {
    GLfloat panams;
    renderTime->pRInfo.secondPass->Program::getUniform("coeffFresnel", &panams);
    panams  = -max(-1.0, -panams-0.05);
    renderTime->pRInfo.secondPass->Program::setUniform1f("coeffFresnel", panams);
    std::cout << "coeffFresnel : " <<  panams <<std::endl;
    break;
  }
  
  case 'r' : 
  {
    rotateLight = true;
    break;
  }
  
  case 'w':
  {
    GLint mode[2];
    glGetIntegerv (GL_POLYGON_MODE, mode);
    glPolygonMode (GL_FRONT_AND_BACK, mode[1] ==  GL_FILL ? GL_LINE : GL_FILL);
    break;
  }

  case 'm' : 
  {
    std::cout << "Voulez vous modifier les paramètres de MipMap(y/n)?" << std::endl;
    string reponse;
    std::cin>>reponse;
    if(reponse=="n")
    {
      std::cout << "Level of mipmap in texture Color used for rendering : " << renderTime->pRInfo.levelColor << std::endl;
      std::cout << "Level of mipmap in texture Normal used for rendering : " << renderTime->pRInfo.levelNormal << std::endl;
      std::cout << "Level of mipmap in texture Position used for rendering : " << renderTime->pRInfo.levelPosition << std::endl;

    }
    else
    {
        string mot;
        std::cout<<"Quelle paramètre ? position/color/normal" << std::endl;
        std::cin >> mot;
        int taille;
        std::cout<<"Quelle niveau de MipMap ? Rappel : width = " << renderTime->pRInfo.width << " height = " << renderTime->pRInfo.height << std::endl;
        std::cin >> taille;
        if(mot == "color" || mot == "c")
        {
          renderTime->pRInfo.levelColor = taille;
          std::cout << "done" << std::endl;
        }
        else if(mot == "position" || mot == "p")
        {
          renderTime->pRInfo.levelPosition = taille;
          std::cout << "done" << std::endl;
        }
        else if(mot == "normal" || mot == "n")
        {
          renderTime->pRInfo.levelNormal = taille;
          std::cout << "done" << std::endl;
        }
    }
    break;
  }
  break;
  default:
  {
    //printUsage ();
    break;
  }
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

  RenderingInfo renderInfo;
  renderInfo.width = DEFAULT_SCREENWIDTH;
  renderInfo.height = DEFAULT_SCREENHEIGHT;
  renderInfo.modelFileName = DEFAULT_MESH_FILE.c_str ();
  renderTime = new Render(renderInfo, &camera, &mesh);

  glutInit (&argc, argv);
  glutInitContextVersion (3, 0);
  // glutInitContextFlags (GLUT_CORE_PROFILE);
  // glutInitContextFlags (GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile (GLUT_CORE_PROFILE);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
  window = glutCreateWindow (appTitle.c_str ());
  std::cout << "init de glew renvoie : " << glewInit() << std::endl;
  renderTime->init ();
  std::cout << "Initialisation réussie" << std::endl;
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

