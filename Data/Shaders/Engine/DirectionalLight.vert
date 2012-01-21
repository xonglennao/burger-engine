//vertex attributes
attribute vec3 vColor;
attribute vec4 vViewSpacePosAndMultiplier;

varying vec3 vVarColor;
varying vec3 vVarLightPos;
varying float fVarMultiplier;
varying float fZMin;

uniform float4x4 mMVP;

void main()
{
	vVarLightPos = vec3( vViewSpacePosAndMultiplier.xyz );
	fVarMultiplier = vViewSpacePosAndMultiplier.a;
	vVarColor = vColor.rgb;

	gl_Position = mMVP * gl_Vertex;
}
