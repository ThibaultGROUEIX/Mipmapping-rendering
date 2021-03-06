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

uniform vec4 matAlbedo;

varying vec4 P;
varying vec3 N; // fragment-wise normal
varying vec4 color;
//varying vec4 roughness;

void main (void)
{
    vec4 fragColor = matAlbedo;
    vec3 n = normalize (gl_NormalMatrix * N);
    
    // ---------- Code to change -------------
    // gl_FragColor = fragColor;
    gl_FragData[1] = matAlbedo; // Ici je stocke la couleur dans ma texture qui est bindée à gl_fragData[1]
    gl_FragData[0] = vec4(n,0.);//0.5*(vec4(n,0.)+vec4(1.0, 1.0, 1.0, 0.0)); //vec4(max(n.x,-n.x),max(n.y,-n.y),max(n.z,-n.z),0.0); //Ici je stocke la normal dans le repère de l'oeil
    gl_FragData[2] = P;
    //gl_FragData[3] = vec4(roughness, 1., 1., 1.);  //Ici je stocke laposition de l'objet dans le repère de l'oeil
 }
 
