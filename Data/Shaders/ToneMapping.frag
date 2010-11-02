uniform vec2 vInvViewport;
uniform sampler2D sTexture;
uniform sampler2D sLuminance;
uniform sampler2D sBloom;

const float fKey = 0.05;

void main()
{
	vec2 vTexCoord = vec2( gl_FragCoord.x * vInvViewport.x, gl_FragCoord.y * vInvViewport.y );

	vec3 vColor = texture2D( sTexture, vTexCoord ).rgb;
	vec3 vBloom = texture2D( sBloom, vTexCoord ).rgb;
	float fLuminance = texture2D( sLuminance, vec2( 0.5, 0.5 ) ).x;

	vColor.rgb *= fKey / ( fLuminance + 0.001 );
	vColor.rgb /= (1.0 + vColor );

	vColor += 3.0 * vBloom; 

	gl_FragColor = vec4( vColor, 1.0 );
}