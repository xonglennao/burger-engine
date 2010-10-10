//vertex attributes
attribute vec4 vColorAndInverseRadius;
attribute vec4 vViewSpacePosAndMultiplier;
attribute float fInverseRadius;
attribute vec3 vViewSpaceDir;
attribute vec2 vCosInAndOut;

varying vec3 vVarColor;
varying vec3 vVarLightPos;
varying float fVarInverseRadius;
varying float fVarMultiplier;
varying float fZMin;
varying vec3 vVarViewSpaceDir;
varying vec2 vVarCosInAndOut;

void main()
{
	vVarLightPos = vec3( vViewSpacePosAndMultiplier.xyz );
	fVarMultiplier = vViewSpacePosAndMultiplier.a;
	vVarColor = vec3( vColorAndInverseRadius.rgb );
	fVarInverseRadius = vColorAndInverseRadius.a;

	fZMin = vVarLightPos.z - 1.0 / fVarInverseRadius;

	vVarViewSpaceDir = vViewSpaceDir;
	vVarCosInAndOut = vCosInAndOut;

	gl_Position = ftransform();
}
