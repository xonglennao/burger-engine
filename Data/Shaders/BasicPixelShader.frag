uniform sampler2D sTexture;

void main()
{
	gl_FragColor = texture2D( sTexture, gl_TexCoord[0].xy );
}