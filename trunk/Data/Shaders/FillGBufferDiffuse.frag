#define LIGHT_COUNT 4

varying vec3 vNormal;

uniform sampler2D diffuseMap; // regular texture: texture unit 0
uniform float fTileSize ; //Used to scale texture coordinates

void main()
{
	vec2 vTexCoord = gl_TexCoord[0].xy * fTileSize;	
	vec3 N = normalize( vNormal );
	N = ( N + 1.0 ) / 2.0;

	gl_FragData[0] = texture2D( diffuseMap, vTexCoord );
	gl_FragData[1] = vec4(N, 0.0);
}
