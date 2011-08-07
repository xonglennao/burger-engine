
attribute vec4 InVertex;  //w will be set to 1.0 automatically
attribute vec4 InColor;
attribute vec2 InSize;
attribute vec2 InTexCoord0;

varying vec2 OutTexCoord0;
varying vec4 OutColor;

uniform mat4 mMVP;
uniform mat4 mInvMVP;
  
void main()
{
	//We need to extract the particle
	//gl_Position = mMVP * InVertex;

	gl_Position = InVertex;
	gl_Position.xy += InSize;
	gl_Position = mMVP * gl_Position;
	
	OutTexCoord0 = InTexCoord0;
	OutColor = InColor;
}