// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------


//Petite explication du code : 
// on a les variables attributes type gl_texCoord qui sont en uniquement en lecture dans le vertex shader.
// Les variable varying sont en général définit dans le vertex shader, et interpolée dand le fragment shader.
// Du coup, ici on tranforme la variable texture gl_texCoord en une m^eme variable mais de type varying


varying vec2 io_texCoord;
varying vec4 P;


void main(void) 
{
	P = gl_Vertex;
	gl_Position = gl_ProjectionMatrix*gl_Vertex;    io_texCoord = gl_TexCoord; //ca à l'air bon
}
