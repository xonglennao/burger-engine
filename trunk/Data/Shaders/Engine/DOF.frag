uniform vec2 vInvViewport;

uniform sampler2D sTexture;
uniform sampler2D sDownSampledTexture;

vec2 poisson[8];
vec2 maxCoC = vec2(5.0,10.0);
float radiusScale = 0.5;

vec4 dof(vec2 coords)
{
	// poisson-distributed positions
	poisson = vec2[8]
	(
		vec2( 0.0, 0.0),
		vec2( 0.527837,-0.085868),
		vec2(-0.040088, 0.536087),
		vec2(-0.670445,-0.179949),
		vec2(-0.419418,-0.616039),
		vec2( 0.440453,-0.639399),
		vec2(-0.757088, 0.349334),
		vec2( 0.574619, 0.685879)
	);
	
	vec4 finalColor;

	vec2 pixelSizeLow = vInvViewport * 2.0;
	
	float discRadius, discRadiusLow, centerDepth;

	centerDepth = texture2D( sTexture, coords ).a;
	discRadius = abs( centerDepth * maxCoC.y - maxCoC.x );
	discRadiusLow = discRadius * radiusScale;

	finalColor = vec4( 0.0, 0.0, 0.0, 0.0 );

	for( int i = 0; i < 8; i++ )
	{
		vec2 coordLow = coords + (pixelSizeLow * poisson[i] * discRadiusLow);
		vec2 coordHigh = coords + ( vInvViewport * poisson[i] * discRadius);

		vec4 tapLow = texture2D( sDownSampledTexture, coordLow );
		vec4 tapHigh = texture2D( sTexture, coordHigh );

		float tapBlur = abs( tapHigh.a * 2.0 - 1.0 );
		vec4 tap = mix( tapHigh, tapLow, tapBlur );

		tap.a = ( tap.a >= centerDepth ) ? 1.0 : abs( tap.a * 2.0 - 1.0 );

		finalColor.rgb += tap.rgb * tap.a;
		finalColor.a += tap.a;
	}

	return finalColor / finalColor.a;
}

void main()
{
	float fColor = texture2D( sTexture, gl_TexCoord[0].xy ).a;
	gl_FragColor = vec4( dof( gl_TexCoord[0].xy ).rgb, 1.0 );	
}