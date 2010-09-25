#define LIGHT_COUNT 4

uniform float fWindowWidth;
uniform float fWindowHeight;

uniform sampler2D sColorSampler;
uniform sampler2D sNormalSampler;
uniform sampler2D sDepthSampler;

uniform mat4 mInvProj;
uniform int bDebug;
void main()
{
	//temporary
	vec4 vSpecColor = vec4(1.0,1.0,1.0,1.0);
	vec4 vAmbient = vec4(0.05,0.05,0.05,1.0);
	
	
	vec2 vTexCoord = vec2( gl_FragCoord.x / fWindowWidth, gl_FragCoord.y / fWindowHeight );

	float fDepth = texture2D( sDepthSampler, vTexCoord ).r;
	vec4 vNormal = texture2D( sNormalSampler, vTexCoord );
	vNormal.xyz = vNormal.xyz * 2.0 - 1.0;
	vec4 vBaseColor = texture2D( sColorSampler, vTexCoord );

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
		vec3 L = normalize( vVertexToLight );			
		float coeffD = max( dot(N,L), 0.0 );
		if(coeffD > 0.0)
		{		
			//attenuation
			float fInverseRadius = gl_LightSource[i].constantAttenuation;				
			float distSqr = dot( vVertexToLight, vVertexToLight );
			float att = clamp(1.0 - fInverseRadius * sqrt(distSqr), 0.0, 1.0);
			att *= att;
			vec3 R = reflect(-L, N);
			vec4 diffuse = coeffD * gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse ;	
			vec4 specular = pow( max( dot( R, E ), 0.0 ), 20 ) * vSpecColor * gl_LightSource[i].specular ;
			finalColor += ( ( diffuse + specular ) * att  );
		}
	}

	if( bDebug == 0 )
		gl_FragColor = finalColor * vBaseColor;
	/*if( bDebug == 1 )
		gl_FragColor = vec4(vNormal.xyz,1.0);
	if( bDebug == 2 )
		gl_FragColor = finalColor * vBaseColor;*/

}