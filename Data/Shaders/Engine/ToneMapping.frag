uniform sampler2D sTexture;
uniform sampler2D sLuminance;

uniform float fToneMappingKey;

void main()
{
	vec3 vColor = texture2D( sTexture, gl_TexCoord[0].xy ).rgb;

	float fLuminance = texture2D( sLuminance, vec2( 0.5, 0.5 ) ).x;

	vColor = 1.0 - exp2(-(fToneMappingKey / ( fLuminance + 0.001 )) * vColor);
	
	//FXAA needs luminance stored in alpha channel
	gl_FragColor = vec4( vColor, dot( vColor, vec3( 0.299, 0.587, 0.114 ) ) );
}