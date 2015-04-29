#ifndef MIPMAP_H
#define MIPMAP_H

class MipMap 
{


public:

	//constructeur
	MipMap(const & int Profondeur);  //Le paramètre profondeur définit le nombre de Texture fille à créer
	//destructeur
	~MipMap();

//méthodes
	void raffiner(); //ajouter un niveau de MipMap

protected:
	GLint location;

	unsigned int width;
	unsigned int height;

	Program * pass;
};


#endif