varying vec4 vPos;

void main()
{
	gl_FragColor = vec4( length( vPos ) * 0.001, 0.0,0.0,0.0 );
}