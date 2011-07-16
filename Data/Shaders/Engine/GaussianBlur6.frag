uniform sampler2D sTexture;
uniform vec2 vPixelSize;



vec4 gaussianFilter()
{
	float vGaussianBlur[6];
	vGaussianBlur[0] = 0.161041;
	vGaussianBlur[1] = 0.16775;
	vGaussianBlur[2] = 0.171209;
	vGaussianBlur[3] = 0.171209;
	vGaussianBlur[4] = 0.16775;
	vGaussianBlur[5] = 0.161041;
	//= { 0.161041, 0.16775, 0.171209, 0.171209, 0.16775, 0.161041 };

	vec4 finalColor;
	vec2 vTexCoord;

	for(int i = 0; i < 6; i++)
	{
		//float fOffSet = (float) i - 2.5;
		float fOffSet = i - 2.5;
		vTexCoord = vec2( gl_TexCoord[0].x + fOffSet * vPixelSize.x, gl_TexCoord[0].y + fOffSet * vPixelSize.y );
		finalColor += texture2D( sTexture, vTexCoord ) * vGaussianBlur[i];
	}
	return finalColor;
}

void main()
{
	gl_FragColor = gaussianFilter();
}
