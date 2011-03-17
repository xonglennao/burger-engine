//vertex attributes
attribute vec3 vColor;
attribute vec4 vViewSpacePosAndMultiplier;

varying vec3 vVarColor;
varying vec3 vVarLightPos;
varying float fVarMultiplier;
varying float fZMin;

void main()
{
	vVarLightPos = vec3( vViewSpacePosAndMultiplier.xyz );
	fVarMultiplier = vViewSpacePosAndMultiplier.a;
	vVarColor = vColor.rgb;

	gl_Position = ftransform();
}
