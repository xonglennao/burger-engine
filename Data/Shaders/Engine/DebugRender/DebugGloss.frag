uniform sampler2D sTexture;

void main()
{
	float fGloss = texture2D( sTexture, gl_TexCoord[0].xy ).a;
	gl_FragColor = vec4( fGloss, fGloss, fGloss, 1.0 );
}