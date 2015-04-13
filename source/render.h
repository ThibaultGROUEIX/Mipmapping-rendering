#ifndef RENDER_H
#define RENDER_H

typedef struct RenderingInfo
{

	// FBO (Une texture, un render buffer et le FBO lui même)
    GLuint texture;
    GLuint renderBuffer;
    GLuint buffer;
    const char * modelFileName;
	// Information sur la fenetre
	unsigned int width;
	unsigned int height;
}RenderingInfo;

char initTexture(GLuint* pTextureID, unsigned int width, unsigned int height);
char initFBO(GLuint* pBuffer, GLuint* pRenderBuffer, GLuint* pTexture, unsigned int width, unsigned int height);


#endif
