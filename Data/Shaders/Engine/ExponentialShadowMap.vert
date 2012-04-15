varying vec4 vPos;
uniform mat4 mMVP;
uniform mat4 mModelView;

void main()
{
	gl_Position = mMVP * gl_Vertex;
	vPos = mModelView * gl_Vertex;
	//vPos = gl_ModelViewMatrix * gl_Vertex;
}
