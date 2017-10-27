varying vec3 normal;
varying vec3 pos;

void main()
{
    gl_Position = ftransform();

	normal = normalize(gl_NormalMatrix * gl_Normal);
	pos = vec3(gl_ModelViewMatrix * gl_Vertex);
    
    gl_FrontColor = gl_Color;
}
