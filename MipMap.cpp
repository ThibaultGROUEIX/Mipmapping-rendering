#include "MipMap.h"

MipMap::pass = new Program::genVFProgram ("MipMap Program", "shaderMipMap.vert.glsl", "shaderMipMap.frag.glsl");


char checkError(const char* placeName)
{
    GLenum glError = glGetError(); // Recupere les erreurs OpenGL
    char message[100] = {'\0'}; // Un peu dangereux, mais supposons que nos messages ne sont pas plus long que 100 char

    assert(placeName != NULL);

    switch (glError)
    {
        case GL_NO_ERROR:
                return 0;
                break;

        case GL_INVALID_ENUM:
            strcpy(message,"Invalid enumerator");
            break;

        case GL_INVALID_VALUE:
            strcpy(message,"Invalid value");
            break;

        case GL_INVALID_OPERATION:
            strcpy(message,"Invalid operation");
            break;

        case GL_OUT_OF_MEMORY:
            strcpy(message,"Out of memory");
            break;

        // Cas special pour les FBO
        case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
            strcpy(message,"Invalid Framebuffer operation");
            break;

        default:
            sprintf(message,"Unknown GL error (0x%ix)",glError);
            break;
    }

    fprintf(stderr,"%s for %s\n",message,placeName);

    return 1;
}



MipMap::MipMap(GLuint FBO_Location, int numberAttachments)
{	

	//a noter : j'ai un pb pour GL_COLOR_ATTACHMENTi...
	GLuint idFBO;
	glGenFrameBuffers(profondeur, &idFBO);
	locations.addLast(idFBO);
	GLuint idsTexture [numberAttachments];

	MipMap::initTextureColor(&idsTexture[0], width/2, height/2);
	MipMap::initTextureColor(&idsTexture[1], width/2, height/2);
	MipMap::initTextureColor(&idsTexture[2], width/2, height/2);
	MipMap::initTextureColor(&idsTexture[3], width/2, height/2);


 // Attachement des donnees au FBO
    glBindFramebuffer(GL_FRAMEBUFFER,idFBO);
    error |= checkError("glBindFramebuffer(FBO)");
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *pTextureNormal, 0);
	error |= checkError("glFramebufferTexture2D");
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, *pTextureNormal, 0);
	error |= checkError("glFramebufferTexture2D");
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, *pTextureNormal, 0);
	error |= checkError("glFramebufferTexture2D");
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, *pTextureNormal, 0);
	error |= checkError("glFramebufferTexture2D");

    glDrawBuffer(GL_COLOR_ATTACHMENT3);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      std::cout << "gros pb" << std::endl;
    }
    //on débind le FBO (machine à état...)
	// Active le rendering dans le FBO (donc, dans une texture)
 	MipMap::pass->use();
    glClearColor(0.0f,0.0f,0.0f,1.0f);

	glBindFramebuffer(GL_FRAMEBUFFER,idFBO);
	GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(3, drawbuffers);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
	glViewport(0,0,width/2,height/2);

	MipMap::pass->use();
	//affecter les variables uniform qui vont bien.

    GLint idTextureColorAttachment0 = glGetUniformLocation(MipMap::pass->id(), "textureColorAttachment0");
    GLint idTextureColorAttachment1 = glGetUniformLocation(MipMap::pass->id(),"textureColorAttachment1");
    GLint idTextureColorAttachment2 = glGetUniformLocation(MipMap::pass->id(),"textureColorAttachment2");
    GLint idTextureColorAttachment3 = glGetUniformLocation(MipMap::pass->id(),"textureColorAttachment3");

    if ( idTextureColorAttachment0 == -1 )
    	fprintf(stderr,"Error while getting the uniform 'textureColorAttachment0'\n");
    if(idTextureColorAttachment0 == -1 )
        fprintf(stderr,"Error while getting the uniform 'textureColorAttachment1'\n");
    if(idTextureColorAttachment2 == -1)
        fprintf(stderr,"Error while getting the uniform 'textureColorAttachment2'\n");
    if(idTextureColorAttachment3 == -1)
        fprintf(stderr,"Error while getting the uniform 'textureColorAttachment3'\n");
    

    //Comment récupérer les handles de ces textures ?

	//Indique la texture au shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,handleTexture0);
	glUniform1i(idTextureDepth,0);
	checkError("glUniform1ui()");

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,handleTexture1);
	glUniform1i(idTextureNormal,1);
	checkError("glUniform1ui()");

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,handleTexture2);
	glUniform1i(idTextureColor,2);
	checkError("glUniform1ui()");

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,handleTexture3);
	glUniform1i(idTexturePosition,3);
	checkError("glUniform1ui()");

	glDisable (GL_CULL_FACE);
	glBegin(GL_QUADS);
	    glColor4f(1, 1, 1, 1);
	    glTexCoord2f(0.0f, 0.0f);
	    glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner
	    glTexCoord2f(0.0f, 1.0f);
	    glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner
	    glTexCoord2f(1.0f, 1.0f);
	    glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner
	    glTexCoord2f(1.0f, 0.0f);
	    glVertex3f(1.0f, -1.0f, 0.0f); // The bottom right corner
	glEnd(); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)

	// On desactive la texture

	glUseProgram(0);
	glEnable (GL_CULL_FACE);
}


char MipMap::initTextureColor(GLuint* pTextureID, unsigned int width, unsigned int height)
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
    std::cout << width << " width " << height << " height " << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    error |= checkError("glTexImage2D");

    //on debind la texture (machine à état)
    glBindTexture(GL_TEXTURE_2D,0);
    error |= checkError("glBindTexture(0)");

    return error;
}