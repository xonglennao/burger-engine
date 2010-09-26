varying vec3 vNormal;

uniform sampler2D diffuseMap; // regular texture: texture unit 0
uniform float fTileSize ; //Used to scale texture coordinates

void main()
{
	vec3 N = normalize( vNormal );
	N = ( N + 1.0 ) / 2.0;

	//gl_FragData[0] = texture2D( diffuseMap, vTexCoord );
	//gl_FragData[1] = vec4(N, 0.0);
	gl_FragColor = vec4(N, 0.0);
}

/***********************************************************/
/***********************************************************/
/*********** DEBUG FORWARD RENDERING ***************/
/***********************************************************/
/***********************************************************/
/*
#define LIGHT_COUNT 4

varying vec3 vNormal, vEye;
varying vec3 vLight[LIGHT_COUNT];


uniform float fTileSize ; //Size of the tile



void main(){

	vec4 vSpecColor = vec4(1.0,1.0,1.0,1.0);
	vec4 vAmbient = vec4(0.05,0.05,0.05,1.0);

	vec2 vTexCoord = gl_TexCoord[0].xy * fTileSize;
	
	//Basic Phong Lighting
	vec3 N = normalize( vNormal );	
	vec3 E = normalize( vEye );

	vec4 finalColor = vAmbient;

	for( int i = 0; i < LIGHT_COUNT; ++i )
	{
			
		vec3 L = normalize( vLight[i] );			

		float coeffD = max( dot(N,L), 0.0 );
		if(coeffD > 0.0)
		{		
			//attenuation
			float fInverseRadius = gl_LightSource[i].constantAttenuation;				
			float distSqr = dot( vLight[i], vLight[i] );
			float att = clamp(1.0 - fInverseRadius * sqrt(distSqr), 0.0, 1.0);
			att *= att;

			vec3 R = reflect(-L, N);
			vec4 diffuse = coeffD * gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse ;	
			vec4 specular = pow( max( dot( R, E ), 0.0 ), 60 ) * vSpecColor * gl_LightSource[i].specular ;
			finalColor += ( ( diffuse + specular ) * att  );
		}
	}
	gl_FragData[0] = finalColor;
	N = ( N + 1.0 ) / 2.0;
	gl_FragData[1] = vec4(N, 1.0);

		
}
*/
/***********************************************************/
/***********************************************************/
/************** END DEBUG FORWARD RENDERING ***************/
/***********************************************************/
/***********************************************************/