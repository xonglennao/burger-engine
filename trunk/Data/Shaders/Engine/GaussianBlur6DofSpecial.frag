uniform sampler2D sTexture;
uniform sampler2D sBlurData;
uniform vec2 vPixelSize;

float vGaussianBlur[6] = { 0.161041, 0.16775, 0.171209, 0.171209, 0.16775, 0.161041 };

vec4 gaussianFilter()
{
	vec4 finalColor;
	vec2 vTexCoord;
	float gaussianValue = 0.0;
	float gaussianTmp = 0.0;

	for(int i = 0; i < 6; i++)
	{
		float fOffSet = (float) i - 2.5;
		
		vTexCoord = vec2( gl_TexCoord[0].x + fOffSet * vPixelSize.x, gl_TexCoord[0].y + fOffSet * vPixelSize.y );
		vec4 vColor = vec4( texture2D( sTexture, vTexCoord ).rgb, texture2D( sBlurData, vTexCoord ).r );
		finalColor += vColor * vGaussianBlur[i];
		/*
		finalColor.rgb += texture2D( sTexture, vTexCoord ).rgb;
		finalColor.a += texture2D( sBlurData, vTexCoord ).r;
		finalColor *= vGaussianBlur[i];
		
		finalColor.rgb += texture2D( sTexture, vTexCoord ).rgb * vGaussianBlur[i];
		*/
	}
	return finalColor;
}

void main()
{
	gl_FragColor = gaussianFilter();
}
