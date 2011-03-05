uniform vec2 vPixelSize;

uniform sampler2D sTexture;

void main()
{
	vec3 vSample = 0.0f;
    float  fLogLumSum = 0.0f;

	for ( float y = -1.0 ; y <= 1.0 ; ++y )
	{
		for ( float x = -1.0 ; x <= 1.0 ; ++x )
		{
			vSample = texture2D( sTexture, vec2( gl_TexCoord[0].x + x * vPixelSize.x, gl_TexCoord[0].y + y * vPixelSize.y ) ).xyz;
			fLogLumSum += log( dot ( vSample, vec3( 0.2126, 0.7152, 0.0722 ) ) + 0.0001f );
		}
	}

    // Divide the sum to complete the average
    fLogLumSum /= 9.0;

    gl_FragColor = vec4(fLogLumSum, fLogLumSum, fLogLumSum, fLogLumSum );
	/*
	vec2 vTexCoord = vec2( gl_FragCoord.x * vPixelSize.x, gl_FragCoord.y * vPixelSize.y );
	
	vec3 vSample = 0.0f;
    float  fLogLumSum = 0.0f;

	for ( float y = -1.0 ; y <= 1.0 ; ++y )
	{
		for ( float x = -1.0 ; x <= 1.0 ; ++x )
		{
			vSample = texture2D( sTexture, vec2( vTexCoord.x + x * vPixelSize.x, vTexCoord.y + y * vPixelSize.y ) ).xyz;
			fLogLumSum += log( dot ( vSample, vec3( 0.2126, 0.7152, 0.0722 ) ) + 0.0001f );
		}
	}

    // Divide the sum to complete the average
    fLogLumSum /= 9.0;

    gl_FragColor = vec4(fLogLumSum, fLogLumSum, fLogLumSum, fLogLumSum );
	*/
}