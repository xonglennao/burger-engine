varying vec3 vNormal;

uniform sampler2D diffuseMap; // regular texture: texture unit 0
uniform float fTileSize ; //Used to scale texture coordinates

void main()
{
	vec2 vTexCoord = gl_TexCoord[0].xy * fTileSize;	
	vec3 N = normalize( vNormal );	

	gl_FragData[0] = texture2D( diffuseMap, vTexCoord );
	gl_FragData[1] = vec4(N, 1.0);
}
