#define LIGHT_COUNT 4


varying vec3 vNormal, vEye;
varying vec3 vLight[LIGHT_COUNT];
varying vec4 vLightSpacePos, vAmbient;

uniform mat4 mEyematrix;
uniform mat4 mTransmatrix;

void main(){

	// vertex position in eye space
	vec4 vVertex = gl_ModelViewMatrix * gl_Vertex;
	
	// vertex position in light space (for shadow mapping)
	vLightSpacePos = mEyematrix * mTransmatrix * gl_Vertex;
	
	// texture coordinates
	gl_TexCoord[0] =  gl_MultiTexCoord0;

	//normal
	vNormal = gl_NormalMatrix * gl_Normal;
	
	//ambiant color
	vAmbient = gl_FrontMaterial.ambient;
	
	for( int i = 0; i < LIGHT_COUNT; ++i )
	{
		vAmbient *= gl_LightSource[i].ambient ;

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
