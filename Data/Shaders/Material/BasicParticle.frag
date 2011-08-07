uniform sampler2D sTexture;

varying vec2 OutTexCoord0;
varying vec4 OutColor;

void main()
{
	//gl_FragColor = OutColor;
	gl_FragColor = texture2D( sTexture, OutTexCoord0.xy );//*OutColor;
}