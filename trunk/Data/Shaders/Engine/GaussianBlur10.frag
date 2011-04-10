uniform sampler2D sTexture;
uniform vec2 vPixelSize;

float vGaussianBlur[10] = { 0.0882357, 0.0957407, 0.101786, 0.106026, 0.108212, 0.108212, 0.106026, 0.101786, 0.0957407, 0.0882357 };

vec4 gaussianFilter()
{
	vec4 finalColor;
	vec2 vTexCoord;

	for(int i = 0; i < 10; i++)
	{
		float fOffSet = (float) i - 4.5;
		
		vTexCoord = vec2( gl_TexCoord[0].x + fOffSet * vPixelSize.x, gl_TexCoord[0].y + fOffSet * vPixelSize.y );
		finalColor += texture2D( sTexture, vTexCoord) * vGaussianBlur[i];
	}
	return finalColor;
}

void main()
{
	gl_FragColor = gaussianFilter();
}
