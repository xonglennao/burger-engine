uniform sampler2D sAdaptedLuminance;
uniform sampler2D sAvgLuminance;

uniform vec2 fElapsedAndBaseTime;

void main()
{
	float fAdaptedLuminance = texture2D( sAdaptedLuminance, vec2( 0.5, 0.5 ) ).x;
	float fAvgLuminance = texture2D( sAvgLuminance, vec2( 0.5, 0.5 ) ).x;
	
	if( fAdaptedLuminance < fAvgLuminance )
	{
		fElapsedAndBaseTime.x *= 0.5;
	}

	float fNewLuminance = fAdaptedLuminance + ( fAvgLuminance - fAdaptedLuminance ) * ( 1.0 - pow( fElapsedAndBaseTime.y, 30.0 * fElapsedAndBaseTime.x ) );

	gl_FragColor = vec4( fNewLuminance, fNewLuminance, fNewLuminance, fNewLuminance );
}