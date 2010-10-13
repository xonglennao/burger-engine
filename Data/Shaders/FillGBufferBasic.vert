varying vec3 vNormal;
void main()
{
	//normal
	vNormal = gl_NormalMatrix * gl_Normal;

	gl_Position = ftransform();
}