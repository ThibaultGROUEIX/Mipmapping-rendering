varying vec4 Pixel;


void main(void) 
{
	Pixel = gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
}