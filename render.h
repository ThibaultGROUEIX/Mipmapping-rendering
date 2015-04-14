#ifndef RENDER_H
#define RENDER_H



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
#include "render.h"


typedef struct RenderingInfo
{

	// FBO (Une texture, un render buffer et le FBO lui même)
	// Ici ce qui m'interesse c'est la depth et la normale, donc j'ai besoin de deux textures ?
    GLuint textureCouleur;
    GLuint textureNormal;
    GLuint textureDepth;
    GLuint buffer;


    const char * modelFileName;
	// Information sur la fenetre
	unsigned int width;
	unsigned int height;

	Program *firstPass;
	Program *secondPass;

}RenderingInfo;

void init (RenderingInfo* pRInfo);
char initTexture(GLuint* pTextureID, unsigned int width, unsigned int height);
char initFBO(GLuint* pBuffer, GLuint* pDepth, GLuint* pTextureNormal, GLuint * pTextureCouleur, unsigned int width, unsigned int height);
void drawScene (RenderingInfo* pRInfo);

#endif
