void main()
{
	// texture coordinates
	gl_TexCoord[0] =  gl_MultiTexCoord0;
	gl_Position = ftransform();
}
