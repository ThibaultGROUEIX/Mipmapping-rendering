//a l'air correct


//Récupération des textures de la première passe

uniform sampler2D fboTexNormal;
uniform sampler2D fboTexDepth;
uniform sampler2D fboTexColor;
uniform sampler2D fboTexPosition;
//recuperation des coordonnées de texture de chaque fragment du vertex shader
//paramètre géré par le CPU
uniform vec4 lightPos;
uniform float coeffFresnel;
uniform float roughness_shader;

uniform mat4 inverseTransposeProjectionMatrix;

varying vec4 P;


//Pi (sans blague!)
const float Pi = 3.14159265359;


void main (void)
{	//récuperation des élements des textures
	float depth = texture2D(fboTexDepth, gl_TexCoord[0]).x;
	vec4 normal = texture2D(fboTexNormal,gl_TexCoord[0]);
	vec4 albedo = texture2D(fboTexColor,gl_TexCoord[0]);
  vec4 pTheorique = texture2D(fboTexPosition,gl_TexCoord[0]);

	//définition des vecteurs pour GGX
  vec4 p = inverseTransposeProjectionMatrix*vec4(P.xy, 2*depth -1, 1.0);
  vec4 p2  = vec4(p.x/p.w, p.y/p.w, p.z/p.w, 1.);
	//vec4 p = vec4(depth,depth,depth,1.);
  //vec4 p = P; 
  vec4 n = normal;
  vec4 l = normalize (- p);


  vec4 v = normalize (-p2);
  vec4 h = normalize(l + v);


    //calcul de GGX Brdf (Il vaudrait mieux la vérifier à mon avis)
    vec4 fragColor = vec4(0.0,0.0,0.0,1.0);
	   float alpha = roughness_shader * roughness_shader;
    vec4 fd = albedo / Pi;
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
    vec4 f = vec4(fs, fs, fs,1.0) + (1.0 - G1 * G2) * fd;
    vec4 color = f*max(0.,dot(n,l));
    // ----------------------------------------
    fragColor += color;

	//Fin GGX

	gl_FragColor = vec4(pTheorique.z,pTheorique.z,pTheorique.z,1.) ;
  //quand je stocke dans la texture couleur, je suis surement clampé par le haut, il faut trouver un meilleur format pour stocker la position!

}
