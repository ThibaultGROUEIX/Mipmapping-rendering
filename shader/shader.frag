//a l'air correct



// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------
// Add here all the value you need to describe the light or the material. 
// At first used const values. 
// Then, use uniform variables and set them from the CPU program.

uniform vec3 matAlbedo;


varying vec3 N; // fragment-wise normal

void main (void)
{
    vec4 fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    vec3 n = normalize (gl_NormalMatrix * N);


    
    // ---------- Code to change -------------


    // gl_FragColor = fragColor;
    gl_FragData[1] = matAlbedo;
    gl_FragData[0] = n;

    //mettre une texture de plus
}
 
