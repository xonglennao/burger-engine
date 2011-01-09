varying mat3 mTBN;

uniform sampler2D normalMap; // regular texture: texture unit 0
void main()
{
	//getting back normal and gloss from normal map
	vec4 vNormalAndGloss = texture2D( normalMap, gl_TexCoord[0].xy );
	vec3 N = vNormalAndGloss.rgb;
	//rescaling
	N = N * 2.0 - 1.0;

	//we need the normal in view space
	N = N * mTBN;	//N*mTBN is the same thing as doing inverse(mTBN)*N
	
	//rescaling again
	N = ( N + 1.0 ) / 2.0;

	gl_FragColor = vec4(N, vNormalAndGloss.a);
}