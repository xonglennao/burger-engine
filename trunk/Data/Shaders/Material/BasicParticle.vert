
attribute vec4 InVertex;  //w will be set to 1.0 automatically
attribute vec4 InColor;
attribute vec2 InSize;
attribute vec2 InTexCoord0;

varying vec2 OutTexCoord0;
varying vec4 OutColor;

uniform mat4 mMVP;
uniform vec2 vInvViewport;
  
void main()
{
	
	//We need to extract the particle
	gl_Position = mMVP * InVertex;

	//gl_Position = InVertex;
	gl_Position.xy += (InSize *vec2(1.0,vInvViewport.y/vInvViewport.x));

	OutTexCoord0 = InTexCoord0;
	OutColor = InColor;
}

/*
vec3 vAt = camPos - gl_Vertex.xyz;
vAt = normalize( vAt );
vec3 vRight = cross( vec3( 0.0, 1.0, 0.0 ), vAt );
vec3 vUp = cross( vAt, vRight );
vRight = normalize( vRight );
vUp = normalize( vUp );


vec2 s = gl_MultiTexCoord0.zw * vec2( size/2.0, size );
vec3 vR = s.xxx * vRight;
vec3 vU = s.yyy * vUp;

vec4 dir = vec4( vR + vU, 0.0 );
gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + dir );

gl_TexCoord[0] = gl_MultiTexCoord0;
*/
/*
    vec4 entPos = texture2D(gpuEntsStates,gl_TexCoord[STATE].st);
03
 
04
    //create the billboard matrix
05
    mat3 billMat;
06
    billMat[2] = normalize(camPos-entPos.xyz); //Z basis
07
    billMat[0] = cross(viewUp,billMat[2]); //X basis
08
    billMat[1] = viewUp; //Y basis
09
 
10
    gl_Position.xyz = billMat*gl_Vertex.xyz; //transform by billboard's orthonormal basis
11
 
12
    gl_Position.xyz += entPos.xyz; //displace
13
    //note, we assume transformation from object into world space transform from the engine is null, since we just did the
14
    //object to world transform above, manually in the shader.
15
    gl_Position = gl_ModelViewProjectionMatrix*gl_Position;

*/

/*
VertexOut Out = (VertexOut)0;
    
    half3 center = mul(input.Position,world);    
    half3 eyeVector = vp._m02_m12_m22;
    
    half3 finalPos = center;
    half3 side;
    half3 up;
    
    side = normalize(cross(eyeVector,worldUp));    
    up = normalize(cross(side,eyeVector));        
    
    finalPos += (input.TextureCoords.x - 0.5) * side * input.Data.x;
    finalPos += (0.5 - input.TextureCoords.y) * up * input.Data.x;
    
    half4 finalPos4 = half4(finalPos,1);    
    
    Out.Position = mul(finalPos4,vp);
    Out.TextureCoords = input.TextureCoords;
    
    Out.Color = input.Color;    
    
    // Alpha
    Out.Color.a = input.Data.y;
    
    return Out;
*/