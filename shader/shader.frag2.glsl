//a l'air correct


//Récupération des textures de la première passe

uniform sampler2D fboTexNormal;
uniform sampler2D fboTexDepth;
uniform sampler2D fboTexColor;

//recuperation des coordonnées de texture de chaque fragment du vertex shader
varying vec2 io_texCoord;
varying vec3 P;
//paramètre géré par le CPU
uniform vec3 lightPos;
uniform float coeffFresnel;
uniform float roughness_shader;

//Pi (sans blague!)
const float Pi = 3.14159265359;


void main (void)
{	//récuperation des élements des textures
	float depth = texture2D(fboTexDepth,io_texCoord).x;
	vec4 normal = texture2D(fboTexNormal,io_texCoord);
	vec4 albedo = texture2D(fboTexColor,io_texCoord);

	//définition des vecteurs pour GGX
	vec3 p = vec3 (gl_ModelViewMatrix * P);
	vec3 n = normal;
    vec3 l = normalize (lightPos - p);
    vec3 v = normalize (-p);
    vec3 h = normalize(l + v);


    //calcul de GGX Brdf
    vec4 fragColor = vec4(0.0,0.0,0.0,1.0);
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

	//Fin GGX

	gl_FragColor = fragColor;

}
