varying vec4 vPos;

void main()
{
	float fDepth = ( length( vPos ) ) - 0.8;
	
	vec2 vMoments;
		
	vMoments.x = fDepth;
	
	float dx = dFdx( fDepth );
	float dy = dFdy( fDepth );
	vMoments.y = fDepth * fDepth + 0.25 * ( dx * dx + dy * dy );
	
	vec4 vFinalColor;
	vFinalColor.rg = frac( vMoments * 64.0 ) / 64.0;
	vFinalColor.ba = vMoments - vFinalColor.rg;

	gl_FragColor = vFinalColor;
}