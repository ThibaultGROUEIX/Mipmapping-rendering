#include "MipMap.h"

//MipMap::pass = Program::genVFProgram ("MipMap Program", "shaderMipMap.vert.glsl", "shaderMipMap.frag.glsl");



MipMap::MipMap(GLuint handleTexture, int _width, int _height) :
width(_width),
height(_height),
niveauMipMap(0)
{
    char error = 0;
    GLuint handleBufferStart;
    GLuint handleBufferEnd;
    glGenFramebuffers(1, &handleBufferStart);
    glGenFramebuffers(1, &handleBufferEnd);
    handleFBOStart = handleBufferStart;
    handleFBOEnd = handleBufferEnd;
    handleTextures = std::vector<GLuint>();
    handleTextures.push_back(handleTexture);
    glBindFramebuffer(GL_FRAMEBUFFER,handleFBOStart);
    // |= check//("glBindFramebuffer(FBO)");
    glBindFramebuffer(GL_FRAMEBUFFER,handleFBOEnd);
    // |= check//("glBindFramebuffer(FBO)");
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    // |= check//("glBindFramebuffer(0)");
    try
    {
        pass = Program::genVFProgram ("MipMap Program", "shaderMipMap.vert.glsl", "shaderMipMap.frag.glsl");
    }
    catch (Exception & e)
    {
    std::cout << e.msg () << std::endl;
        std::cout << "Je suis la" << std::endl;

    }
    if (error != 0)
    {
        std::cout << "pb d'initialisation du FBO" << std::endl;
    }
    //MipMap, paramètre statique est déjà affecté.
}

void MipMap::SetLevel(const int& level)
{
    niveauMipMap = level;
}

void MipMap::raffiner(int i)
{   
    char error = 0;
    if(i <= niveauMipMap || i == 0)
    {
        return;
    }
    else
    {
        for(int j = 1; j<i+1; j++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER,handleFBOStart);
            // |= check//("glBindFramebuffer(FBO)");
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handleTextures[j-1], 0);
            // |= check//("glFramebufferTexture2D");
        	

            GLuint idNewTexture;
        	MipMap::initTextureColor(&idNewTexture, width/(pow(2,j)), height/(pow(2,j)));

            handleTextures.push_back(idNewTexture);
         // Attachement des donnees au FBO
            glBindFramebuffer(GL_FRAMEBUFFER,handleFBOEnd);
            // |= check//("glBindFramebuffer(FBO)");
        	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handleTextures[j], 0);
        	// |= check//("glFramebufferTexture2D");

            
            //on débind le FBO (machine à état...)
        	// Active le rendering dans le FBO (donc, dans une texture)
         	MipMap::pass->use();
            glClearColor(0.0f,0.0f,0.0f,1.0f);

        	glBindFramebuffer(GL_FRAMEBUFFER,handleFBOEnd);
        	GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0 };
        	glDrawBuffers(1, drawbuffers);
        	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
        	glViewport(0,0,width/pow(2,j),height/pow(2,j));

        	//affecter les variables uniform qui vont bien.

            GLint idTextureColorAttachment0 = glGetUniformLocation(MipMap::pass->id(), "textureColorAttachment0");

            if ( idTextureColorAttachment0 == -1 )
            	fprintf(stderr,"// while getting the uniform 'textureColorAttachment0'\n");
            
        	//Indique la texture au shader
        	glActiveTexture(GL_TEXTURE0);
        	glBindTexture(GL_TEXTURE_2D,handleTextures[0]);
        	glUniform1i(idTextureColorAttachment0,0);
        	//check//("glUniform1ui()");


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
            niveauMipMap++;
        	glEnable (GL_CULL_FACE);
            if(error!=0)
            {
                std::cout << "//" << std::endl;
            }
        }
    }
}



void MipMap::use(int i)
{   
    if(i>niveauMipMap)
    {
        std::cout << "you ask for a MipMap level which is not generated yet" << std::endl;
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER,handleFBOEnd);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handleTextures[i], 0);
        GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawbuffers);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }
}


char MipMap::initTextureColor(GLuint* pTextureID, unsigned int width, unsigned int height)
{
    char error = 0;

    assert(pTextureID);

    //pTextureID va contenir l'index de la texture
    glGenTextures(1,pTextureID);
    // |= check//("glGenTextures");

    //on bind la texture pour faire des manip (machine à état...)
    glBindTexture(GL_TEXTURE_2D,*pTextureID);
    // |= check//("glBindTexture(texId)");

    // Definit les paramètre de la texture
    // Si ce n'est pas fait, la texture n'est pas valide
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // |= check//("glTexParameterf");
    // On alloue un espace pour les futures donnees
    // ici on définit le format de la texture, ici ca va pas puisque moi je veux récupérer les normales et la depth donc la couleur m'interresse pas...
    //std::cout << width << " width " << height << " height " << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // |= check//("glTexImage2D");

    //on debind la texture (machine à état)
    glBindTexture(GL_TEXTURE_2D,0);
    // |= check//("glBindTexture(0)");

    return error;
}


MipMap::~MipMap()
{

}