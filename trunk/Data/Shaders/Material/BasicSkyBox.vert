varying vec3 vPos;
uniform float4x4 mMVP;
void main(void)
{
	vPos = vec3( gl_Vertex );
	gl_Position = mMVP * gl_Vertex;
}
