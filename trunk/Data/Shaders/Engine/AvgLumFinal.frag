uniform vec2 vPixelSize;

uniform sampler2D sTexture;

void main()
{
	float fColor = 0.0;

	for ( float y = -1.5 ; y <=1.5 ; ++y )
	{
		for ( float x = -1.5 ; x <=1.5 ; ++x )
		{
			fColor += texture2D( sTexture, vec2( gl_TexCoord[0].x + x * vPixelSize.x, gl_TexCoord[0].y + y * vPixelSize.y ) ).x;
		}
	}

	fColor = exp( fColor / 16.0 );
	gl_FragColor = vec4( fColor, fColor, fColor, fColor );
	
	/*
	vec2 vTexCoord = vec2( gl_FragCoord.x * vPixelSize.x, gl_FragCoord.y * vPixelSize.y );
	
	float fColor = 0.0;

	for ( float y = -1.5 ; y <=1.5 ; ++y )
	{
		for ( float x = -1.5 ; x <=1.5 ; ++x )
		{
			fColor += texture2D( sTexture, vec2( vTexCoord.x + x * vPixelSize.x, vTexCoord.y + y * vPixelSize.y ) ).x;
		}
	}

	fColor = exp( fColor / 16.0 );
	gl_FragColor = vec4( fColor, fColor, fColor, fColor );
	*/
}