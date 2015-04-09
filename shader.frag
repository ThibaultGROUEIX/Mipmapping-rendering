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

const vec3 lightPos = vec3 (5.0, 5.0, 5.0);
const vec3 matAlbedo = vec3 (0.6, 0.6, 0.6);
const float Pi = 3.14159265359;
float coeffFresnel = 0.90;
uniform float roughness_shader;

varying vec4 P; // fragment-wise position
varying vec3 N; // fragment-wise normal

void main (void)
{
    vec4 fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    
    vec3 p = vec3 (gl_ModelViewMatrix * P);
    vec3 n = normalize (gl_NormalMatrix * N);
    vec3 l = normalize (lightPos - p);
    vec3 v = normalize (-p);
    vec3 h = normalize(l + v);

    
    // ---------- Code to change -------------
    float alpha = roughness_shader * roughness_shader;
    vec3 fd = matAlbedo / Pi;
    float coef = 1.0 + (alpha - 1.0) * dot(n, h) * dot(n, h);
    float D = alpha / (Pi * coef * coef);
    
    float F = coeffFresnel +
              (1.0 - coeffFresnel) *
              pow(1.0 - max(0.0, dot(l, h)), 5.0);
    
    float G1 = 2.0 * dot(n, l) / (dot(n, l) +
               sqrt(alpha + (1.0 - alpha) *
               dot(n, l) * dot(n, l)));

    float G2 = 2.0 * dot(n, v) /
               (dot(n, v) + sqrt(alpha + (1.0 - alpha) * dot(n, v) * dot(n, v)));

    float fs = D * F * G1 * G2 / (4.0 * dot (n, l) * dot(n, v));
    vec3 f = vec3(fs, fs, fs) + (1.0 - G1 * G2) * fd;
    vec4 color = vec4 (f, 1.0);
    // ----------------------------------------
    fragColor += color;

    gl_FragColor = fragColor;
}
 
