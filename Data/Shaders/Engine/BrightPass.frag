uniform vec2 vInvViewport;
uniform sampler2D sTexture;
uniform sampler2D sLuminance;

uniform vec3 fThresholdOffSetKey;

void main()
{
	vec2 vTexCoord = vec2( gl_FragCoord.x * vInvViewport.x, gl_FragCoord.y * vInvViewport.y );

	vec3 vColor = texture2D( sTexture, vTexCoord ).rgb;
	float fLuminance = texture2D( sLuminance, vec2( 0.5, 0.5 ) ).x;
	
	// Determine what the pixel's value will be after tone mapping occurs
	vColor.rgb *= fThresholdOffSetKey.z / ( fLuminance + 0.001 );

	// Subtract out dark pixels
	vColor.rgb -= fThresholdOffSetKey.x;

	// Clamp to 0
	vColor = max( vColor, 0.0 );

	// Map the resulting value into the 0 to 1 range. Higher values for
	// BRIGHT_PASS_OFFSET will isolate lights from illuminated scene 
	// objects.
	vColor.rgb /= ( fThresholdOffSetKey.y + vColor );

	gl_FragColor = vec4( vColor,1.0 );
}