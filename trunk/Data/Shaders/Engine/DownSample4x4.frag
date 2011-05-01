uniform vec2 vPixelSize;

uniform sampler2D sTexture;

void main()
{
	vec4 vColor = vec4( 0.0 );

	for ( float y = -1.5 ; y <=1.5 ; ++y )
	{
		for ( float x = -1.5 ; x <=1.5 ; ++x )
		{
			vColor += texture2D( sTexture, vec2( gl_TexCoord[0].x + x * vPixelSize.x, gl_TexCoord[0].y + y * vPixelSize.y ) );
		}
	}
	gl_FragColor = vColor / 16.0;
}