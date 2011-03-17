uniform sampler2D lightBuffer; // light accumulation buffer: texture unit 6

uniform vec4 vColor;

uniform vec2 vInvViewport;

uniform vec4 vDofParams;
//x = near blur; y = focal plane; z = far blur; w = blurriness cutoff

float ComputeDepthBlur( float fDepth )
{

	float f;

	if( fDepth < vDofParams.y )
	{
		f = ( fDepth - vDofParams.y ) / ( vDofParams.y - vDofParams.x );
		f = clamp( f, -1.0, 0.0);
	}
	else
	{
		f = ( fDepth - vDofParams.y ) / ( vDofParams.z - vDofParams.y );
		f = clamp( f, 0.0, vDofParams.w );
	}
	
	return f * 0.5 + 0.5;

}

void main()
{
	vec2 vTexCoord = gl_TexCoord[0].xy;
	//we need screen space coordinates in order to get sample from the GBuffer
	vec2 vScreenTexCoord = vec2( gl_FragCoord.x * vInvViewport.x, gl_FragCoord.y * vInvViewport.y );

	vec4 vLighting = texture2D( lightBuffer, vScreenTexCoord );


	//reconstructing specular
	vec3 vChromacity = clamp( vLighting.rgb / ( dot( vLighting.rgb, vec3(0.2126, 0.7152, 0.0722 )  ) + 0.00001 ), 0.0, 1.0 );

	gl_FragData[0] = vec4( (vLighting.rgb + vChromacity * vLighting.a ) * vColor.rgb ,1.0);

	//Computing blur information for Depth of Field
	float fDepth = gl_FragCoord.z / gl_FragCoord.w;	
	gl_FragData[1] = vec4( ComputeDepthBlur( fDepth ), 0.0, 0.0, 0.0);
}