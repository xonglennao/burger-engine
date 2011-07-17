uniform sampler2D sAdaptedLuminance;
uniform sampler2D sAvgLuminance;

uniform vec2 fElapsedAndBaseTime;

void main()
{
	float fAdaptedLuminance = texture2D( sAdaptedLuminance, vec2( 0.5, 0.5 ) ).x;
	float fAvgLuminance = texture2D( sAvgLuminance, vec2( 0.5, 0.5 ) ).x;
	
	// Copy needed, we cannot modify an uniform
	vec2 f2ElapsedAndBaseTimeCopy = fElapsedAndBaseTime;
	
	if( fAdaptedLuminance < fAvgLuminance )
	{
		f2ElapsedAndBaseTimeCopy.x = f2ElapsedAndBaseTimeCopy.x*0.5;
	}

	float fNewLuminance = fAdaptedLuminance + ( fAvgLuminance - fAdaptedLuminance ) * ( 1.0 - pow( f2ElapsedAndBaseTimeCopy.y, 30.0 * f2ElapsedAndBaseTimeCopy.x ) );
	
	gl_FragColor = vec4( fNewLuminance );
}