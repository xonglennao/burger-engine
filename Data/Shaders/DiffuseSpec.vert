uniform float fTileSize ; //Used to scale texture coordinates
void main()
{
	// texture coordinates
	gl_TexCoord[0] =  gl_MultiTexCoord0 * fTileSize;
	gl_Position = ftransform();
}
