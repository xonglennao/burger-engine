varying vec3 vNormal;
void main()
{
	// texture coordinates
	//gl_TexCoord[0] =  gl_MultiTexCoord0;

	//normal
	vNormal = gl_NormalMatrix * gl_Normal;

	gl_Position = ftransform();
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
varying vec4 vLightSpacePos;

void main(){

	// vertex position in eye space
	vec4 vVertex = gl_ModelViewMatrix * gl_Vertex;
	
	// texture coordinates
	gl_TexCoord[0] =  gl_MultiTexCoord0;

	//normal
	vNormal = gl_NormalMatrix * gl_Normal;
	
	
	for( int i = 0; i < LIGHT_COUNT; ++i )
	{
		//from vertex to light
		vLight[i] = gl_LightSource[i].position.xyz;
		//point or spot light
		if(gl_LightSource[i].position.w == 1.0)
			vLight[i] -= vVertex.xyz;
	}

	//from vertex to eye
	vEye =  -vVertex.xyz;

	gl_Position = ftransform();
}
/***********************************************************/
/***********************************************************/
/*********** END DEBUG FORWARD RENDERING ***************/
/***********************************************************/
/***********************************************************/
