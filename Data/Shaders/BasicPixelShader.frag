uniform vec2 vInvViewport;
uniform sampler2D sTexture;

void main()
{
	vec2 vTexCoord = vec2( gl_FragCoord.x * vInvViewport.x, gl_FragCoord.y * vInvViewport.y );

	gl_FragColor = texture2D( sTexture, vTexCoord );
}