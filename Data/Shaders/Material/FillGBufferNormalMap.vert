varying mat3 mTBN;
uniform float fTileSize ; //Used to scale texture coordinates
uniform mat4 mMVP;
uniform mat4 mModelView;

void main()
{
	// texture coordinates
	gl_TexCoord[0] =  gl_MultiTexCoord0 * fTileSize;

	//Getting back the Tangent
	vec3 T = normalize( gl_MultiTexCoord1.xyz ); 
	
	//Creating TBN Matrix
	T = normalize( (mModelView * vec4(T,0.0)).xyz );
	vec3 N = normalize( (mModelView * vec4(gl_Normal,0.0)).xyz );
	vec3 B = normalize( cross( N,  T ));

	mTBN[0][0] = T[0]; mTBN[1][0] = T[1]; mTBN[2][0] = T[2];
	mTBN[0][1] = B[0]; mTBN[1][1] = B[1]; mTBN[2][1] = B[2];
	mTBN[0][2] = N[0]; mTBN[1][2] = N[1]; mTBN[2][2] = N[2];

	gl_Position = mMVP * gl_Vertex;
}
