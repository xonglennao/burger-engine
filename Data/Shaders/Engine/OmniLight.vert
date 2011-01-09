//vertex attributes
attribute vec4 vColorAndInverseRadius;
attribute vec4 vViewSpacePosAndMultiplier;
attribute float fInverseRadius;

varying vec3 vVarColor;
varying vec3 vVarLightPos;
varying float fVarInverseRadius;
varying float fVarMultiplier;
varying float fZMin;

void main()
{
	vVarLightPos = vec3( vViewSpacePosAndMultiplier.xyz );
	fVarMultiplier = vViewSpacePosAndMultiplier.a;
	vVarColor = vec3( vColorAndInverseRadius.rgb );
	fVarInverseRadius = vColorAndInverseRadius.a;

	fZMin = vVarLightPos.z - 1.0 / fVarInverseRadius;

	gl_Position = ftransform();
}
