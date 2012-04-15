varying vec3 vPos;
uniform mat4 mMVP;
void main(void)
{
	vPos = vec3( gl_Vertex );
	gl_Position = mMVP * gl_Vertex;
}
