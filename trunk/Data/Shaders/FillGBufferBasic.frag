varying vec3 vNormal;

void main()
{
	vec3 N = normalize( vNormal );
	N = ( N + 1.0 ) / 2.0;
	gl_FragColor = vec4(N, 0.5);
}