varying vec3 normal;
varying vec3 pos;
varying vec4 shadowTexCoord;

void main()
{
    gl_Position = ftransform();

	normal = normalize(gl_NormalMatrix * gl_Normal);
	pos = vec3(gl_ModelViewMatrix * gl_Vertex);
    
    gl_FrontColor = gl_Color;

	shadowTexCoord = gl_TextureMatrix[1] * gl_Vertex;
}
