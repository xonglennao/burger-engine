varying vec4 vPos;
uniform float4x4 mMVP;
uniform float4x4 mModelView;

void main()
{
	gl_Position = mMVP * gl_Vertex;
	vPos = mModelView * gl_Vertex;
	//vPos = gl_ModelViewMatrix * gl_Vertex;
}
