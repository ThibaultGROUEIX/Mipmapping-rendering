//Récupération des textures de la première passe

uniform sampler2D fboTexNormal;
uniform sampler2D fboTexColor;
uniform sampler2D fboTexPosition;
//uniform sampler2D fboTexDepth;
//recuperation des coordonnées de texture de chaque fragment du vertex shader
//paramètre géré par le CPU
uniform vec4 lightPos;
uniform float coeffFresnel;
uniform float roughness_shader;

//uniform mat4 inverseTransposeProjectionMatrix;

//Pi (sans blague!)
const float Pi = 3.14159265359;


void main (void)
{	//récuperation des élements des textures
	//float depth = texture2D(fboTexDepth, gl_TexCoord[0]).x;
	vec4 normal = texture2D(fboTexNormal,gl_TexCoord[0]);
	vec4 albedo = texture2D(fboTexColor,gl_TexCoord[0]);
  vec4 positionTheorique = texture2D(fboTexPosition,gl_TexCoord[0]);

  	// //définition des vecteurs pour GGX
   //  vec4 positionFromDepth = inverseTransposeProjectionMatrix*vec4(Pixel.xy, 2*depth -1, 1.0);
   //  vec4 p2  = vec4(positionFromDepth.x/positionFromDepth.w, positionFromDepth.y/positionFromDepth.w, positionFromDepth.z/positionFromDepth.w, 1.);
   //  positionFromDepth = p2;

  vec4 n =  normal; 
  vec4 l =  (gl_ModelViewMatrix*lightPos - positionTheorique);
  vec4 v = = (positionTheorique);

  vec3 nR  = n;
  vec3 lR  = normalize(vec3 ( l.x / l.w, l.y / l.w, l.z / l.w )); 
  vec3 vR  = normalize(vec3 ( v.x / v.w, v.y / v.w, v.z / v.w )); 
  vec3 hR  = normalize(lR + vR);

  //calcul de GGX Brdf (Il vaudrait mieux la vérifier à mon avis)
  vec4 fragColor = vec4(0.0,0.0,0.0,1.0);
  float alpha = roughness_shader * roughness_shader;
  vec4 fd = albedo / Pi;
  float coef = 1.0 + (alpha - 1.0) * dot(nR, hR) * dot(nR, hR);
  float D = alpha / (Pi * coef * coef);
  
  float F = coeffFresnel +
            (1.0 - coeffFresnel) *
            pow(1.0 - max(0.0, dot(lR, hR)), 5.0);
  
  float G1 = 2.0 * dot(nR, lR) / (dot(nR, lR) +
             sqrt(alpha + (1.0 - alpha) *
             dot(nR, lR) * dot(nR, lR)));

  float G2 = 2.0 * dot(nR, vR) /
             (dot(nR, vR) + sqrt(alpha + (1.0 - alpha) * dot(nR, vR) * dot(nR, vR)));

  float fs = albedo*D * F * G1 * G2 / (4.0 * dot (nR, lR) * dot(nR, vR));
  vec4 f = vec4(fs, fs, fs,1.0) + (1.0 - G1 * G2) * fd;
  vec4 color = f*max(0.,dot(nR,lR));
  // ----------------------------------------
  fragColor += color;

	//Fin GGX

	gl_FragColor = normal;
  //quand je stocke dans la texture couleur, je suis surement clampé par le haut, il faut trouver un meilleur format pour stocker la position!

}
