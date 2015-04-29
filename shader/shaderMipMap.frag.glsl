//a l'air correct


//Récupération des textures de la première passe

uniform sampler2D textureColorAttachment0;
uniform sampler2D textureColorAttachment1;
uniform sampler2D textureColorAttachment2;
uniform sampler2D textureColorAttachment3;
//recuperation des coordonnées de texture de chaque fragment du vertex shader

varying vec4 Pixel;
void main (void)
{	
  vec4 newFrag0;
  vec4 newFrag1;
  vec4 newFrag2;
  vec4 newFrag3;

  //récuperation des élements des textures
	vec4 fragColorAttachment0 = texture2D(textureColorAttachment0,gl_TexCoord[0]);
	vec4 fragColorAttachment1 = texture2D(textureColorAttachment1,gl_TexCoord[0]);
  vec4 fragColorAttachment2 = texture2D(textureColorAttachment2,gl_TexCoord[0]);
  vec4 fragColorAttachment3 = texture2D(textureColorAttachment3,gl_TexCoord[0]);

// pixel fetch
  texelFetch 2*gl_FragCoord(gl_TexCoord[0]+(0,0))
//Ici se passe les choses interressantes !


  //On renvoie
  gl_FragData[0] = newFrag0;
  gl_FragData[1] = newFrag1;
  gl_FragData[2] = newFrag2;
  gl_FragData[3] = newFrag3;


}
