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
#include <cassert>
#include "Vec3.h"
#include "Camera.h"
#include "Mesh.h"
#include "GLProgram.h"
#include <cstring>
#include <string>
#include "Matrices.h"
#include "MipMap.h"

char checkError(const char* placeName);



typedef struct RenderingInfo
{

	// FBO (Une texture, un render buffer et le FBO lui même)
	// Ici ce qui m'interesse c'est la depth et la normale, donc j'ai besoin de deux textures ?
    GLuint textureCouleur;
    GLuint textureNormal;
    GLuint textureDepth;
    GLuint texturePosition;

    MipMap * mipColor;
    MipMap * mipNormal;
    MipMap * mipPosition;

    GLuint buffer;
    Mesh * mesh;
    Camera * camera;


    const char * modelFileName;
	// Information sur la fenetre
	unsigned int width;
	unsigned int height;

	Program *firstPass;
	Program *secondPass;

}RenderingInfo;


class Render {
public : 
	//constructeur
	Render( RenderingInfo const &  _pRInfo, Camera * const  camera, Mesh * const  mesh);
	//destructeur
	~Render();
	char init ();
	char initTextureColour(GLuint* pTextureID, unsigned int width, unsigned int height);
	char initTextureDepth(GLuint* pTextureID, unsigned int width, unsigned int height);

	char initFBO(GLuint* pBuffer, GLuint* pDepth, GLuint* pTextureNormal, GLuint * pTextureCouleur,GLuint * pTexturePosition, unsigned int width, unsigned int height);
	void drawScene ();
	void displayScreen();
	void GenerateGBuffer();
	void ComputeBRDF(const int& levelColor, const int& levelPosition, const int& levelAlbedo);

public :
	RenderingInfo pRInfo;

};



#endif
