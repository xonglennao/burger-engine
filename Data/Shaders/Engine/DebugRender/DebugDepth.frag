uniform sampler2D sTexture;

void main()
{
	float fDepth = (1.0 - texture2D( sTexture, gl_TexCoord[0].xy ).x) * 100.0;

	gl_FragColor = vec4( fDepth, fDepth,fDepth,1.0 );
}