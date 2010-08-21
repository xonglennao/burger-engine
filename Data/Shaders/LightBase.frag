#define LIGHT_COUNT 4

varying vec3 vNormal, vEye;
varying vec3 vLight[LIGHT_COUNT];
varying vec4 vLightSpacePos, vAmbient;

uniform sampler2D diffuseMap; // regular texture: texture unit 0
uniform sampler2D shadowMap; // depth map

uniform float fShininess; //material parameter
uniform float fTileSize ; //Size of the tile

uniform bool bUseShadowMap;

/*******************************************/
/*****		Main				 ***********/
/*******************************************/
void main(){

	vec2 vTexCoord = gl_TexCoord[0].xy * fTileSize;
	
	float shadow = 1.0;	
	float spot = 1.0;
	if( false ){
		vec3 shadowCoordinateWdivide;
		shadowCoordinateWdivide = vLightSpacePos.xyz / vLightSpacePos.w ;
		shadowCoordinateWdivide = shadowCoordinateWdivide * 0.5 + 0.5;

		float distanceFromLight = texture2D(shadowMap,shadowCoordinateWdivide.st).z;
		distanceFromLight += 0.00005;
 		
 		if ( vLightSpacePos.w > 0.0)
 			shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.0 : 1.0 ;
 	}
			
	//Basic Phong Lighting
	vec3 N = normalize( vNormal );	
	vec3 E = normalize( vEye );

	vec4 finalColor = vAmbient;
	
	//if fragment is in shadow, we don't compute lighting
	if( shadow != 1.0 ){	
		gl_FragColor = finalColor * texture2D( diffuseMap, vTexCoord );
	}
	else{
		for( int i = 0; i < LIGHT_COUNT; ++i )
		{
			
			spot = 1.0;
			
			vec3 L = normalize( vLight[i] );//			

			/*********************************/
			/******** Spotlight *************/
			/*******************************/
			if(/*gl_LightSource[i].spotCutoff != 180.0*/ false )
			{
				float fCosOut = gl_LightSource[i].quadraticAttenuation;
				
				vec3 D = normalize( gl_LightSource[i].spotDirection );
				float cos_cur_angle = dot( -L, D );
				float cos_inner_cone_angle = gl_LightSource[i].spotCosCutoff;
				float cos_inner_minus_outer_angle = cos_inner_cone_angle - fCosOut;
				spot = clamp( ( cos_cur_angle - fCosOut ) / cos_inner_minus_outer_angle, 0.0, 1.0 );
			}
				
			/*******************************/
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
				vec4 specular = pow( max( dot( R, E ), 0.0 ), fShininess ) * gl_FrontMaterial.specular * gl_LightSource[i].specular ;
				finalColor += ( ( diffuse + specular ) * spot * att  );
			}
		}
		gl_FragColor = finalColor  * texture2D( diffuseMap, vTexCoord );
	}
		
}
