// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

varying vec3 N;
varying vec4 P;

// le vertex shader renvoie obligatoirement gl_Position
// le vertex shader prend en argument. Idem on transforme une variable attribute en une variable varying qui sera interpolée.
// soyons malin, les lights sont prédéfinie en opengl, le peu récupérer light 0 en utilisant "g lL i g h t S o u r c e  [ 0 ] .  p o s i t i o n . x y z" (vex3)

void main(void) 
{	
	P = gl_ModelViewMatrix*gl_Vertex;
    N = gl_Normal; //ne puis-ja pas appeller gl_normal dans le fragment shader ?
    gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
    //gl_Position = ftransform();	
}
