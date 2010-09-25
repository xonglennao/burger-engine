varying vec3 vNormal;

void main(){
	// texture coordinates
	gl_TexCoord[0] =  gl_MultiTexCoord0;

	//normal
	vNormal = gl_NormalMatrix * gl_Normal;
	
	gl_Position = ftransform();
}
