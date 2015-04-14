#include "render.h"



void init (RenderingInfo* pRInfo) {



  char error = 0;
  assert(pRInfo);

    
  //Initialisation du FBO

    error |= initTextureColour(&pRInfo->textureCouleur,pRInfo->width,pRInfo->height);
    error |= initTextureColour(&pRInfo->textureNormal,pRInfo->width,pRInfo->height);
    error |= initTextureDepth(&pRInfo->textureDepth,pRInfo->width,pRInfo->height);

    //init la deusieme texture
    error |= initFBO(&pRInfo->buffer,&pRInfo->textureDepth, &pRInfo->textureNormal, &pRInfo->textureCouleur, pRInfo->width, pRInfo->height);


  //initialisation random

  glewInit();
  glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
  glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
  glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
  glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
  glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color
	

  //initialisation du Mesh
  camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT); // Setup the camera
  mesh.loadOFF (pRinfo->modelFilename); // Load a mesh file


  try {

  //initialisation du Premier Programme
  pRInfo->firstPass = Program::genVFProgram ("Simple fist pass Program", "shader.vert", "shader.frag"); // Load and compile pair of shaders
  pRInfo->firstPass->use (); // Activate the shader program
  pRInfo->firstPass->Program::setUniform1f("roughness_shader", 0.3);
  pRInfo->firstPass->Program::setUniform1f("coeffFresnel", 0.04);
  pRInfo->firstPass->Program::setUniform3f("lightPos", 0.5,0.5,0.5);
  pRInfo->firstPass->Program::setUniform3f("matAlbedo",0.5,0.5,0.5);

  //initialisation  du Second Programme
  pRInfo->secondPass = Program::genVFProgram ("Simple second pass Program", "shader.vert2", "shader.frag2"); // Load and compile pair of shaders


  } catch (Exception & e) {
    cerr << e.msg () << endl;
  }

  return error;

}
char initTextureColour(GLuint* pTextureID, unsigned int width, unsigned int height)
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
    // ici on définit le format de la texture, ici ca va pas puisque moi je veux récupérer les normales et la depth donc la couleur m'interresse pas...
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    error |= checkError("glTexImage2D");

    //on debind la texture (machine à état)
    glBindTexture(GL_TEXTURE_2D,0);
    error |= checkError("glBindTexture(0)");

    return error;
}

char initTextureDepth(GLuint* pTextureID, unsigned int width, unsigned int height)
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
    // ici on définit le format de la texture, ici ca va pas puisque moi je veux récupérer les normales et la depth donc la couleur m'interresse pas...

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    error |= checkError("glTexImage2D");

    //on debind la texture (machine à état)
    glBindTexture(GL_TEXTURE_2D,0);
    error |= checkError("glBindTexture(0)");

    return error;
}

char initFBO(GLuint* pBuffer, GLuint* pDepth, GLuint* pTextureNormal, GLuint * pTextureCouleur, unsigned int width, unsigned int height)
{
    char error = 0;

    assert(pBuffer);
    assert(pRenderBuffer);
    assert(pTexture);
    //on génère un FBO, dont l'index est contenu dans pBuffer
    glGenFramebuffers(1, pBuffer);

  

    // Attachement des donnees au FBO
    glBindFramebuffer(GL_FRAMEBUFFER,*pBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *pDepth);
    error |= checkError("glFramebufferTexture2D");
    error |= checkError("glBindFramebuffer(FBO)");
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *pTextureNormal, 0);
    error |= checkError("glFramebufferTexture2D");
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, *pTextureCouleur, 0);
    error |= checkError("glFramebufferTexture2D");
    glDrawBuffer(GL_COLOR_ATTACHMENT1);


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

  pRInfo->firstPass->use();
  if(rotateLight)
  {  
    rotateLightAngle+=0.05;
    glFirstPass->Program::setUniform3f("lightPos", cos(rotateLightAngle),0.5,sin(rotateLightAngle));
  }
  glBegin (GL_TRIANGLES);
  for (unsigned int i = 0; i < mesh.T.size (); i++) 
    for (unsigned int j = 0; j < 3; j++) {
      const Vertex & v = mesh.V[mesh.T[i].v[j]];
      //glattrib manuel
      //glTexCoor...
      //glColor3f()
      glNormal3f (v.n[0], v.n[1], v.n[2]); // Specifies current normal vertex   
      glVertex3f (v.p[0], v.p[1], v.p[2]); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)
    }
  glEnd ();
  glUseProgram(0);
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
    GLint idTextureNormal = glGetUniformLocation(pRInfo->programShaderP2,"fboTexNormal");
    GLint idTextureDepth = glGetUniformLocation(pRInfo->programShaderP2,"fboTexColor");
    GLint idTextureColor = glGetUniformLocation(pRInfo->programShaderP2,"fboTexDepth");


    if ( idTextureNormal == -1 || idTextureColor == -1 || idTextureDepth == -1)
    {
        fprintf(stderr,"Error while getting the uniform 'fboTex'\n");
    }
    //pareil depth
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
      glBindTexture(GL_TEXTURE_2D,pRInfo->textureDepth);
      glUniform1i(idTextureDepth,0);
      checkError("glUniform1ui()");
      
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D,pRInfo->textureNormal);
      glUniform1i(idTextureNormal,1);
      checkError("glUniform1ui()");

      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D,pRInfo->textureCouleur);
      glUniform1i(idTextureColor,2);
      checkError("glUniform1ui()");

      // Affichage d'un gros cube
      ICI FAUT SE MEFIER
      glDisable (GL_CULL_FACE);
      glBegin(GL_TRIANGLES);
   
          glTexCoor2d(1.0,0.0);
          glVertex3f (1.0,-1.0,0.0);
          glTexCoor2d(1.0,1.0);
          glVertex3f (1.0,1.0,0.0);
          glTexCoor2d(0.0,1.0);
          glVertex3f (-1.0,1.0,0.0);

          glTexCoor2d(0.0,1.0);
          glVertex3f (-1.0,1.0,0.0);
          glTexCoor2d(0.0,0.0);
          glVertex3f (-1.0,-1.0,0.0);
          glTexCoor2d(1.0,0.0);
          glVertex3f (1.0,-1.0,0.0);

      glEnd(); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)
    
      // On desactive la texture

      glBindTexture(GL_TEXTURE_2D,0);
      glUseProgram(0);
      glEnable (GL_CULL_FACE);
}

      // Ajout d'un affichage de debug, prouvant que le FBO fonctionne
      glBindFramebuffer(GL_READ_FRAMEBUFFER,pRInfo->textureCouleur);
      glBlitFramebuffer(0,0,pRInfo->width,pRInfo->height,
                        0,0,pRInfo->width/3,pRInfo->height/3,
                        GL_COLOR_BUFFER_BIT,
                        GL_LINEAR);


}


// gldrawbuffer (idem gl active texture)  espèce d'équivalent de glActiveTexture au moment de l'initialisation  debase seul le premier color attachment est actif donc il faut traité ce pb 
//regarder GL_view_port quelle va ^etre la corelation entre u fragment et un pixel
//il faut lui donner 0,0 height, width
//voir comment se passe l'interpolation des variables varying
// quelle diff entre définir une variable dans le main et à l'extérieur ?
// voir comment je peux passer en argument des attribut non prévu par opengl
