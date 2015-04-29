#ifndef MIPMAP_H
#define MIPMAP_H


#include "render.h"
#include "GLProgram.h"
#include <vector>;

class MipMap 
{


public:

	//constructeur
	MipMap(const & int profondeur, GLuint FBO_Location, int numberAttachments);  //Le paramètre profondeur définit le nombre de Texture fille à créer
	//destructeur
	~MipMap();

//méthodes
	void raffiner(); //ajouter un niveau de MipMap

//méthode static
	static char initTextureColor(GLuint* pTextureID, unsigned int width, unsigned int height);


protected:
	vector<GLint> locations;

	unsigned int width;
	unsigned int height;

	static Program * pass;
};


#endif