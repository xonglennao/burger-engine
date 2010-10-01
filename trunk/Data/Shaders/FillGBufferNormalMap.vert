varying mat3 mTBN;
uniform float fTileSize ; //Used to scale texture coordinates
void main()
{
	// texture coordinates
	gl_TexCoord[0] =  gl_MultiTexCoord0 * fTileSize;

	//Getting back the Tangent
	vec3 T = normalize( gl_MultiTexCoord1.xyz ); 
	
	//We are putting those into the world space, int order to create a matrix
	//that will convert light position and vertex position into the TBN space
	T = normalize( gl_NormalMatrix * T );
	vec3 N = normalize( gl_NormalMatrix * gl_Normal );
	vec3 B = normalize( cross( N,  T ));

	mTBN[0][0] = T[0]; mTBN[1][0] = T[1]; mTBN[2][0] = T[2];
	mTBN[0][1] = B[0]; mTBN[1][1] = B[1]; mTBN[2][1] = B[2];
	mTBN[0][2] = N[0]; mTBN[1][2] = N[1]; mTBN[2][2] = N[2];

	gl_Position = ftransform();
}
