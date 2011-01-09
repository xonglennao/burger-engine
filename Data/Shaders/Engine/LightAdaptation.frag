uniform sampler2D sAdaptedLuminance;
uniform sampler2D sAvgLuminance;

uniform float fElapsedTime;

void main()
{
	float fAdaptedLuminance = texture2D( sAdaptedLuminance, vec2( 0.5, 0.5 ) ).x;
	float fAvgLuminance = texture2D( sAvgLuminance, vec2( 0.5, 0.5 ) ).x;
	
	/*if( ( fAdaptedLuminance - fAvgLuminance ) < 0.0 )
	{
		fElapsedTime *= 0.1;
	}*/

	float fNewLuminance = fAdaptedLuminance + ( fAvgLuminance - fAdaptedLuminance ) * ( 1.0 - pow( 0.95, 30.0 * fElapsedTime ) );

	gl_FragColor = vec4( fNewLuminance, fNewLuminance, fNewLuminance, fNewLuminance );
}