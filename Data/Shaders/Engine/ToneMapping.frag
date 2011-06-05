uniform sampler2D sTexture;
uniform sampler2D sLuminance;
uniform sampler2D sBloom;
uniform sampler2D sBlurData;
uniform sampler3D sLUT;

uniform vec2 fGlowMultiplierAndKey;

void main()
{
	vec3 vColor = texture2D( sTexture, gl_TexCoord[0].xy ).rgb;
	
	vec3 vBloom = texture2D( sBloom, gl_TexCoord[0].xy ).rgb;
	float fLuminance = texture2D( sLuminance, vec2( 0.5, 0.5 ) ).x;

	vColor.rgb *= fGlowMultiplierAndKey.y / ( fLuminance + 0.001 );
	vColor.rgb /= ( 1.0 + vColor );

	vColor += fGlowMultiplierAndKey.x * vBloom;
	
	gl_FragColor = vec4( texture3D( sLUT, vColor ).rgb, texture2D( sBlurData, gl_TexCoord[0].xy ).x );	
}