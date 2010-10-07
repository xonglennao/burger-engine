uniform sampler2D diffuseMap; // regular texture: texture unit 0
uniform sampler2D lightBuffer; // light from GBuffer: texture unit 6

uniform vec2 vInvViewport;

void main()
{
	vec2 vTexCoord = gl_TexCoord[0].xy;
	//we need screen space coordinates in order to get sample from the GBuffer
	vec2 vScreenTexCoord = vec2( gl_FragCoord.x * vInvViewport.x, gl_FragCoord.y * vInvViewport.y );

	vec4 vLighting = texture2D( lightBuffer, vScreenTexCoord );

	//reconstructing specular
	vec3 vChromacity = clamp( vLighting.rgb / ( dot( vLighting.rgb, vec3(0.2126, 0.7152, 0.0722 ) ) + 0.00001f ), 0.0, 1.0 );

	gl_FragColor = vec4( (vLighting.rgb + vChromacity * vLighting.a ),1.0) * texture2D( diffuseMap, vTexCoord );
}