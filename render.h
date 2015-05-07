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
#include "tiny_obj_loader.h"

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

    int levelColor;
    int levelNormal;
    int levelPosition;


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
	char init (const std::string & filename);
	char initTextureColour(GLuint* pTextureID, unsigned int width, unsigned int height);
	char initTextureDepth(GLuint* pTextureID, unsigned int width, unsigned int height);

	char initFBO(GLuint* pBuffer, GLuint* pDepth, GLuint* pTextureNormal, GLuint * pTextureCouleur,GLuint * pTexturePosition, unsigned int width, unsigned int height);
	void drawScene ();
	void displayScreen(const int& levelC, const int& levelN, const int& levelP);

	//void displayScreen();
	void GenerateGBufferFromMesh();
	void GenerateGBufferFromObject();
	void ComputeBRDF(const int& _levelColor, const int& _levelPosition, const int& _levelNormal);
	void computeSceneNormals ();
	void computeSceneBoundingSphere ();
	bool loadScene(const std::string & filename, const std::string & basepath = "");



public :
	RenderingInfo pRInfo;
	static Vec3f sceneCenter;
	static float sceneRadius;
	static std::vector<tinyobj::shape_t> shapes;
	static std::vector<tinyobj::material_t> materials;
	bool loadingBool;

};



#endif
