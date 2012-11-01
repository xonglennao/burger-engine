uniform vec2 vInvViewport;

uniform sampler2D sNormalSampler;
uniform sampler2D sDepthSampler;

uniform sampler2D sShadowMapSampler;

uniform mat4 mInvProj;
uniform mat4[4] mShadowMatrices;

uniform vec4[4] vSphere;

varying vec3 vVarColor;
varying vec3 vVarLightPos;
varying float fVarMultiplier;
varying float fZMin;

void main()
{
	vec4 vColors[4];
	vColors[0] = vec4(0.2,0.0,0.0,1.0);
	vColors[1] = vec4(0.0,0.2,0.0,1.0);
	vColors[2] = vec4(0.0,0.0,0.2,1.0);
	vColors[3] = vec4(1.0,1.0,1.0,1.0);
	
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

	vec4 vColor = vec4(1.0);

	vec4 vComp = vec4( length(vViewSpaceVertex.xyz - vSphere[0].xyz)<=vSphere[0].w,length(vViewSpaceVertex.xyz - vSphere[1].xyz)<=vSphere[1].w,length(vViewSpaceVertex.xyz - vSphere[2].xyz)<=vSphere[2].w,length(vViewSpaceVertex.xyz - vSphere[3].xyz)<=vSphere[3].w );
	
	for( int i = 3; i >0; --i )
	{
		vComp[i] = max((vComp[i]-vComp[i-1]),0.0);
	}

	float fCompResult = dot(vComp,vec4(1.0,2.0,3.0,4.0));
	float fIndex = max(fCompResult - 1.0,0.0);
	//This is an ungly fix but we do not have a choice
	//cg compiler need an Integer between bracket, no float - it's not supported since 1.3 version
	int iIndex = int(fIndex);
	vColor = vColors[iIndex];

	vec4 vLightSpaceVertex = mShadowMatrices[iIndex] * vClipPos;
	vLightSpaceVertex = ( vLightSpaceVertex / vLightSpaceVertex.w ) * 0.5 + 0.5;
	
	vec2 vShadowCoords = vec2( vLightSpaceVertex.x*0.25+(fIndex*0.25), vLightSpaceVertex.y );
	float fShadowSample = texture2D( sShadowMapSampler, vShadowCoords ).x;
	float fShadow = float(fShadowSample > vLightSpaceVertex.z);
	fShadow += float( fCompResult == 0);
	fShadow = min(fShadow,1.0);

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
	finalColor += vec4(diffuse, 4.0 * fSpecularLuminance );
	

	gl_FragColor = finalColor*fShadow;
	
	//gl_FragColor = finalColor*fShadow*vColor;
}