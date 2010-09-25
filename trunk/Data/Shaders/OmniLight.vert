#define LIGHT_COUNT 4
varying vec3 vLight[LIGHT_COUNT];
void main()
{
	for( int i = 0; i < LIGHT_COUNT; ++i )
	{
		vLight[i] = gl_LightSource[i].position.xyz;
	}
	
	gl_Position = ftransform();
}
