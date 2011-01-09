varying vec4 vPos;

void main()
{
	gl_Position = ftransform();
	vPos = gl_ModelViewMatrix * gl_Vertex;
}
