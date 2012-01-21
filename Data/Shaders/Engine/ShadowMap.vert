uniform mat4x4 mMVP;

void main()
{
	gl_Position = mMVP * gl_Vertex;
}
