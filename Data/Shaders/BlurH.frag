uniform sampler2D texture;

uniform vec2 vInvViewport;

float sX, sY;

float PI = 3.14159265358979323846264;

int blurValue = 3;

float lambda = 1.0; 
float sigma = 25.0;

float gaussian(float x)
{
	return (lambda/(sigma*sqrt(2.0*PI)))*exp(-0.5*pow(x/sigma,2.0));
}

vec4 gaussianFilter1DHorizontal(int radius)
{
	vec4 finalColor;
	vec2 pos;
	float gaussianValue = 0.0;
	float gaussianTmp = 0.0;
	int index = 0;

	for(int i = -radius; i <= radius; i++)
	{
		pos = vec2(gl_TexCoord[0].x+float(i)*sX,gl_TexCoord[0].y);
		gaussianTmp =  gaussian(i);
		gaussianValue += gaussianTmp;
		finalColor += texture2D(texture, pos) * gaussianTmp;
		index ++;
	}

	return finalColor/gaussianValue;
}

void main()
{
	sX = 1.0/512.0;
	gl_FragColor = gaussianFilter1DHorizontal(blurValue);
}
