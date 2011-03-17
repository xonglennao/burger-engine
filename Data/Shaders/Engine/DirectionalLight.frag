uniform vec2 vInvViewport;

uniform sampler2D sNormalSampler;
uniform sampler2D sDepthSampler;

uniform mat4 mInvProj;

varying vec3 vVarColor;
varying vec3 vVarLightPos;
varying float fVarMultiplier;
varying float fZMin;

void main()
{
	vec4 finalColor = vec4(0.0,0.0,0.0,0.0);
	
	//we need the screen-space position of the fragment in order to fetch the GBuffer
	vec2 vTexCoord = vec2( gl_FragCoord.x * vInvViewport.x, gl_FragCoord.y * vInvViewport.y );

	//we need the view-space position of the vertex
	float fDepth = texture2D( sDepthSampler, vTexCoord ).r;
	
	// Construct screen-space position
	vec4 vClipPos = vec4( vTexCoord.x * 2.0 - 1.0, vTexCoord.y * 2.0 - 1.0, fDepth * 2.0 - 1.0, 1.0 );
	// Multiply by inverse projection matrix to get view-space position
	vec4 vViewSpaceVertex = mInvProj * vClipPos;
	vViewSpaceVertex = vViewSpaceVertex / abs(vViewSpaceVertex.w);

	vec4 vNormalAndGloss = texture2D( sNormalSampler, vTexCoord );
	vNormalAndGloss.xyz = vNormalAndGloss.xyz * 2.0 - 1.0;	

	//Phong Lighting
	vec3 N = normalize( vNormalAndGloss.xyz );
	vec3 E = normalize( -vViewSpaceVertex.xyz );

	vec3 L = normalize( vVarLightPos.xyz );		
	float NDotL = max( dot(N,L), 0.0 ) * fVarMultiplier;

	vec3 R = reflect(-L, N);

	vec3 diffuse = NDotL * vVarColor;

	float fSpecular = pow( max( dot( R, E ), 0.0 ), vNormalAndGloss.w * 256.0 ) * NDotL;
	float fSpecularLuminance = dot( vec3(fSpecular,0.0,0.0), vec3( 0.2126, 0.7152, 0.0722 ) );

	//storing diffuse and specular on different channels (rgb = diffuse, a = lum(spec) ) 
	finalColor += vec4(diffuse, 4.0f * fSpecularLuminance );

	gl_FragColor = finalColor;
}