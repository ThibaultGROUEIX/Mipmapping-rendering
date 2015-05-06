#ifndef MIPMAP_H
#define MIPMAP_H


#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <cassert>
#include "GLProgram.h"
#include <vector>
#include <iostream>

class MipMap 
{


public:

	//constructeur
	//création des deux FBO, assignation des attribus
	MipMap(GLuint handleTexture, int width, int height);  //Le paramètre profondeur définit le nombre de Texture fille à créer
	//destructeur
	~MipMap();

//méthodes
	//test si niveau est plus grand que niveau MipMap et update
	void SetLevel(const int& level);
	void raffiner(int niveau); //ajouter un niveau de MipMap
	void use(int niveau);

//méthode static
	//pour générer un nouvelle texture
	static char initTextureColor(GLuint* pTextureID, unsigned int width, unsigned int height);



public:
	GLuint handleFBOStart;
	GLuint handleFBOEnd;
	std::vector<GLuint> handleTextures;

	unsigned int width;
	unsigned int height;
	unsigned int niveauMipMap;

	Program * pass;
};


#endif