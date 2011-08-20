uniform sampler2D sTexture;

varying vec2 OutTexCoord0;
varying vec4 OutColor;
/*
float ComputeDepthBlur( float fDepth )
{
	float f;

	if( fDepth < vDofParams.y )
	{
		f = ( fDepth - vDofParams.y ) / ( vDofParams.y - vDofParams.x );
		f = clamp( f, -1.0, 0.0);
	}
	else
	{
		f = ( fDepth - vDofParams.y ) / ( vDofParams.z - vDofParams.y );
		f = clamp( f, 0.0, vDofParams.w );
	}
	
	return f * 0.5 + 0.5;
}
*/
void main()
{
	//gl_FragColor = OutColor;
	gl_FragColor = texture2D( sTexture, OutTexCoord0.xy );//*OutColor;
}