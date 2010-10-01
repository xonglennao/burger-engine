varying mat3 mTBN;

uniform sampler2D normalMap; // regular texture: texture unit 0
void main()
{
	//getting back normal from normal map
	vec3 N = texture2D( normalMap, gl_TexCoord[0].xy );
	//rescaling
	N = N * 2.0 - 1.0;

	//we need the normal in view space
	N = N * mTBN;	//N*mTBN is the same thing as doing inverse(mTBN)*N
	
	//rescaling again
	N = ( N + 1.0 ) / 2.0;

	//gl_FragData[0] = texture2D( diffuseMap, gl_TexCoord[0].xy );
	//gl_FragData[1] = vec4(N, 0.0);
	gl_FragColor = vec4(N, 0.0);
}