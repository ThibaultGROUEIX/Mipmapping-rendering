void init (RenderingInfo* pRInfo) {



  char error = 0;

  assert(pRInfo);

    
  //Initialisation du FBO

    error |= initTexture(&pRInfo->texture,pRInfo->width,pRInfo->height);
    error |= initFBO(&pRInfo->buffer,&pRInfo->renderBuffer, &pRInfo->texture, pRInfo->width, pRInfo->height);


  //initialisation random

  glewInit();
  glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
  glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
  glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
  glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
  glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
	

  //initialisation du Mesh
  camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT); // Setup the camera
  mesh.loadOFF (pRinfo->modelFilename); // Load a mesh file


  try {

  //initialisation du Premier Programme
  glFirstPass = Program::genFirstPassProgram ("Simple fist pass Program", "shader.vert1", "shader.frag1"); // Load and compile pair of shaders
  glFirstPass->use (); // Activate the shader program
  glFirstPass->Program::setUniform1f("roughness_shader", 0.3);
  glFirstPass->Program::setUniform1f("coeffFresnel", 0.04);
  glFirstPass->Program::setUniform3f("lightPos", 0.5,0.5,0.5);
  glFirstPass->Program::setUniform3f("matAlbedo",0.5,0.5,0.5);

  //initialisation  du Second Programme
  glSecondPass = Program::genSecondPassProgram ("Simple second pass Program", "shader.vert2", "shader.frag2"); // Load and compile pair of shaders


  } catch (Exception & e) {
    cerr << e.msg () << endl;
  }

  return error;

}
char initTexture(GLuint* pTextureID, unsigned int width, unsigned int height)
{
    char error = 0;

    assert(pTextureID);

    //pTextureID va contenir l'index de la texture
    glGenTextures(1,pTextureID);
    error |= checkError("glGenTextures");

    //on bind la texture pour faire des manip (machine à état...)
    glBindTexture(GL_TEXTURE_2D,*pTextureID);
    error |= checkError("glBindTexture(texId)");

    // Definit les paramètre de la texture
    // Si ce n'est pas fait, la texture n'est pas valide
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    error |= checkError("glTexParameterf");
    // On alloue un espace pour les futures donnees
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    error |= checkError("glTexImage2D");

    //on debind la texture (machine à état)
    glBindTexture(GL_TEXTURE_2D,0);
    error |= checkError("glBindTexture(0)");

    return error;
}

char initFBO(GLuint* pBuffer, GLuint* pRenderBuffer, GLuint* pTexture, unsigned int width, unsigned int height)
{
    char error = 0;

    assert(pBuffer);
    assert(pRenderBuffer);
    assert(pTexture);
    //on génère un FBO, dont l'index est contenu dans pBuffer
    glGenFramebuffers(1, pBuffer);

    // Creation du render buffer
    glGenRenderbuffers(1, pRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER,*pRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
    error |= checkError("glRenderbufferStorage");
    glBindRenderbuffer(GL_RENDERBUFFER,0);

    // Attachement des donnees au FBO
    glBindFramebuffer(GL_FRAMEBUFFER,*pBuffer);
    error |= checkError("glBindFramebuffer(FBO)");
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *pTexture, 0);
    error |= checkError("glFramebufferTexture2D");
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *pRenderBuffer);
    error |= checkError("glFramebufferRenderbuffer");

    //on débind le FBO (machine à état...)
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    error |= checkError("glBindFramebuffer(0)");

    return error;
}



void drawScene (RenderingInfo* pRInfo) {
//First Pass
  {
  // Active le rendering dans le FBO (donc, dans une texture)
  glBindFramebuffer(GL_FRAMEBUFFER,pRInfo->buffer);

  // Efface le buffer
  glClearColor(0.4f,0.0f,0.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );

  glFirstPass->use();
  if(rotateLight)
  {  
    rotateLightAngle+=0.05;
    glFirstPass->Program::setUniform3f("lightPos", cos(rotateLightAngle),0.5,sin(rotateLightAngle));
  }
  glBegin (GL_TRIANGLES);
  for (unsigned int i = 0; i < mesh.T.size (); i++) 
    for (unsigned int j = 0; j < 3; j++) {
      const Vertex & v = mesh.V[mesh.T[i].v[j]];
      glNormal3f (v.n[0], v.n[1], v.n[2]); // Specifies current normal vertex   
      glVertex3f (v.p[0], v.p[1], v.p[2]); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)
    }
  glEnd ();
  glUseProgram(0);
  // va falloir remplir de facon adéquat mon ptit gars. 
}
//second Pass
{    // On passe sur l'ecran
    glBindFramebuffer(GL_FRAMEBUFFER,0);


    // Efface le buffer
    glClearColor(0.0f,0.4f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );

    GLint idPosition;
    GLint idTextureCoords;

    // Recuperation de l'id de l'uniform
    GLint idTexture = glGetUniformLocation(pRInfo->programShaderP2,"fboTex");

    if ( idTexture == -1 )
    {
        fprintf(stderr,"Error while getting the uniform 'fboTex'\n");
    }

    glSecondPass->use();

    // Recuperation des id pour les attributes
      idPosition = glGetAttribLocation(pRInfo->programShaderP2,"position");
      if ( idPosition == -1 )
      {
          fprintf(stderr,"Error while getting the attrib 'position'\n");
      }
      checkError("glGetAttribLocation(position)");

      idTextureCoords = glGetAttribLocation(pRInfo->programShaderP2,"texCoord");
      if ( idTextureCoords == -1 )
      {
          fprintf(stderr,"Error while getting the attrib 'texCoord'\n");
      }
      checkError("glGetAttribLocation(texCoord)");


      // Indique la texture au shader
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D,pRInfo->texture);
      glUniform1i(idTexture,0);
      checkError("glUniform1ui()");
      
      // Affichage d'un gros cube
      glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f( 100.0f, 100.0f, -100.0f);
      glVertex3f(-100.0f, 100.0f, -100.0f);
      glVertex3f(-100.0f, 100.0f,  100.0f);
      glVertex3f( 100.0f, 100.0f,  100.0f);
 
      // Bottom face (y = -100.0f)
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( 100.0f, -100.0f,  100.0f);
      glVertex3f(-100.0f, -100.0f,  100.0f);
      glVertex3f(-100.0f, -100.0f, -100.0f);
      glVertex3f( 100.0f, -100.0f, -100.0f);
 
      // Front face  (z = 100.0f)
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( 100.0f,  100.0f, 100.0f);
      glVertex3f(-100.0f,  100.0f, 100.0f);
      glVertex3f(-100.0f, -100.0f, 100.0f);
      glVertex3f( 100.0f, -100.0f, 100.0f);
 
      // Back face (z = -100.0f)
      glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      glVertex3f( 100.0f, -100.0f, -100.0f);
      glVertex3f(-100.0f, -100.0f, -100.0f);
      glVertex3f(-100.0f,  100.0f, -100.0f);
      glVertex3f( 100.0f,  100.0f, -100.0f);
 
      // Left face (x = -100.0f)
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-100.0f,  100.0f,  100.0f);
      glVertex3f(-100.0f,  100.0f, -100.0f);
      glVertex3f(-100.0f, -100.0f, -100.0f);
      glVertex3f(-100.0f, -100.0f,  100.0f);
 
      // Right face (x = 100.0f)
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glVertex3f(100.0f,  100.0f, -100.0f);
      glVertex3f(100.0f,  100.0f,  100.0f);
      glVertex3f(100.0f, -100.0f,  100.0f);
      glVertex3f(100.0f, -100.0f, -100.0f);
   glEnd(); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)
    
  }
  glEnd ();
      // On desactive la texture
      glBindTexture(GL_TEXTURE_2D,0);

      glUseProgram(0);
}

      // Ajout d'un affichage de debug, prouvant que le FBO fonctionne
      glBindFramebuffer(GL_READ_FRAMEBUFFER,pRInfo->buffer);
      glBlitFramebuffer(0,0,pRInfo->width,pRInfo->height,
                        0,0,pRInfo->width/3,pRInfo->height/3,
                        GL_COLOR_BUFFER_BIT,
                        GL_LINEAR);


}


//Qu'est envoyer comme vertex à la seconde passe
//faire le generer deuxieme Programme
