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




void main(void) 
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
}
