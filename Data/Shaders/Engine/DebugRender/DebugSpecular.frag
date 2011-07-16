uniform sampler2D sTexture;

void main()
{
	vec4 vLighting = texture2D( sTexture, gl_TexCoord[0].xy );
	vec3 vSpecular = clamp( vLighting.rgb / ( dot( vLighting.rgb, vec3(0.2126, 0.7152, 0.0722 )  ) + 0.00001 ), 0.0, 1.0 ) * vLighting.a;

	gl_FragColor = vec4( vSpecular, 1.0 );
}