#ifndef MATH_MATRICES_H
#define MATH_MATRICES_H

class MipMap {


public:


//constructeur
MipMap(const & int Profondeur);  //Le paramètre profondeur définit le nombre de Texture fille à créer
//destructeur
~MipMap();


protected:
	GLint location;
}





#endif