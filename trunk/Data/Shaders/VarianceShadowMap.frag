varying vec4 vPos;

void main()
{
	float fDepth = length( vPos ) -0.8;
		
	float fMoment1 = fDepth;
	
	float dx = dFdx( fDepth );
	float dy = dFdy( fDepth );

	float fMoment2 = fDepth * fDepth + 0.25 * ( dx * dx + dy * dy );
		
	gl_FragColor = vec4( fMoment1,fMoment2,0.0, 0.0 );
}