varying vec3 vPos;
void main(void)
{
	vPos = vec3( gl_Vertex );
	gl_Position = ftransform();
}
