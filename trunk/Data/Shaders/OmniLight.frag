#define LIGHT_COUNT 4

uniform float fWindowWidth;
uniform float fWindowHeight;

uniform sampler2D sNormalSampler;
uniform sampler2D sDepthSampler;

uniform mat4 mInvProj;
uniform int iDebug;
void main()
{
	//temporary
	vec4 vSpecColor = vec4(1.0,1.0,1.0,0.0);
	vec4 vAmbient = vec4(0.05,0.05,0.05,0.0);	
	
	vec2 vTexCoord = vec2( gl_FragCoord.x / fWindowWidth, gl_FragCoord.y / fWindowHeight );

	float fDepth = texture2D( sDepthSampler, vTexCoord ).r;
	vec4 vNormal = texture2D( sNormalSampler, vTexCoord );

	vNormal.xyz = vNormal.xyz * 2.0 - 1.0;
	// Construct clip-space position
	vec4 vClipPos = vec4( 2.0 * vTexCoord.x - 1.0, 2.0 * vTexCoord.y - 1.0, 2.0 * fDepth - 1.0, 1.0 );
	// Multiply by inverse projection matrix to get eye-space position
	vec4 vEyePos = mInvProj * vClipPos;
	vEyePos = vEyePos/vEyePos.w;
	
	//Basic Phong Lighting
	vec3 N = normalize(vNormal.xyz);	
	vec3 E = normalize(-vEyePos.xyz);
	vec4 finalColor = vAmbient;

	for( int i = 0; i < LIGHT_COUNT; ++i )
	{
		vec3 vVertexToLight = gl_LightSource[i].position.xyz - vEyePos.xyz;
						
		//attenuation
		float fInverseRadius = gl_LightSource[i].constantAttenuation;				
		float fDistSqr = dot( vVertexToLight, vVertexToLight );
		float fAtt = clamp( 1.0 - fInverseRadius * sqrt(fDistSqr), 0.0, 1.0 );
		fAtt *= fAtt;

		vec3 L = normalize( vVertexToLight );		
		float NDotLAtt = max( dot(N,L), 0.0 ) * fAtt;

		vec3 R = reflect(-L, N);

		vec3 diffuse = NDotLAtt * gl_LightSource[i].diffuse.rgb;
			
		float fSpecular = pow( max( dot( R, E ), 0.0 ), 60 );
		float fSpecularLuminance = dot( fSpecular, float3( 0.2126, 0.7152, 0.0722 ) );

		finalColor += vec4(diffuse, fSpecularLuminance * NDotLAtt );
	}

	gl_FragColor = finalColor;
}