#version 130
#extension GL_EXT_gpu_shader4 : enable

precision mediump float;

uniform sampler2D sTexture;

uniform sampler2D sBloom;
uniform sampler2D sBlurData;
uniform sampler3D sLUT;

uniform float fGlowMultiplier;

uniform vec2 vInvViewport;
uniform int iDebug;


// Original FXAA code by Timothy Lottes/NVidia : http://timothylottes.blogspot.com/2011/06/fxaa3-source-released.html

/*============================================================================
                         FXAA QUALITY - TUNING KNOBS
============================================================================*/
#ifndef FXAA_QUALITY__EDGE_THRESHOLD
    //
    // The minimum amount of local contrast required to apply algorithm.
    //
    // 1/3 - too little
    // 1/4 - low quality
    // 1/6 - default
    // 1/8 - high quality
    // 1/16 - overkill
    //
    #define FXAA_QUALITY__EDGE_THRESHOLD (1.0/8.0)
#endif
/*--------------------------------------------------------------------------*/
#ifndef FXAA_QUALITY__EDGE_THRESHOLD_MIN
    //
    // Trims the algorithm from processing darks.
    //
    // 1/32 - visible limit
    // 1/16 - high quality
    // 1/12 - upper limit (default, the start of visible unfiltered edges)
    //
    #define FXAA_QUALITY__EDGE_THRESHOLD_MIN (1.0/16.0)
#endif
/*--------------------------------------------------------------------------*/
#ifndef FXAA_QUALITY__SUBPIX_CAP
    //
    // Insures fine detail is not completely removed.
    // This partly overrides FXAA_SUBPIX_TRIM.
    //
    // 3/4 - default amount of filtering
    // 7/8 - high amount of filtering
    // 1 - no capping of filtering
    //
    #define FXAA_QUALITY__SUBPIX_CAP (3.0/4.0)
#endif
/*--------------------------------------------------------------------------*/
#ifndef FXAA_QUALITY__SUBPIX_TRIM
    //
    // Controls removal of sub-pixel aliasing,
    //
    // 1/2 - low removal (sharper but more sub-pixel aliasing)
    // 1/3 - medium removal
    // 1/4 - default removal
    // 1/8 - high removal
    // 0 - complete removal (softer but less sub-pixel aliasing)
    //
    #define FXAA_QUALITY__SUBPIX_TRIM (1.0/4.0)
#endif

/*============================================================================

                                 API PORTING
                                 
============================================================================*/

#define FxaaSat(x) clamp(x, 0.0, 1.0)
#define FxaaTexTop(t, p) textureLod(t, p, 0.0)
#define FxaaTexOff(t, p, o, r) textureLodOffset(t, p, 0.0, o)

/*============================================================================

                              FXAA3 QUALITY - PC

============================================================================*/

/*--------------------------------------------------------------------------*/
vec3 FxaaPixelShader( vec2 pos, sampler2D tex, vec2 rcpFrame )
{   
	/*--------------------------------------------------------------------------*/
    float lumaN = FxaaTexOff(tex, pos.xy, ivec2(0, -1), rcpFrame.xy).w;
    float lumaW = FxaaTexOff(tex, pos.xy, ivec2(-1, 0), rcpFrame.xy).w;
	vec4 rgbyM = FxaaTexTop(tex, pos.xy);
	float lumaE = FxaaTexOff(tex, pos.xy, ivec2( 1, 0), rcpFrame.xy).w;
    float lumaS = FxaaTexOff(tex, pos.xy, ivec2( 0, 1), rcpFrame.xy).w;
    float lumaM = rgbyM.w;
	/*--------------------------------------------------------------------------*/
    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
    float range = rangeMax - rangeMin;
	/*--------------------------------------------------------------------------*/
    if( range < max( FXAA_QUALITY__EDGE_THRESHOLD_MIN, rangeMax * FXAA_QUALITY__EDGE_THRESHOLD ) )
	{
		return rgbyM.rgb;//vec4(0.0,0.0,0.0,0.0);
	}
	/*--------------------------------------------------------------------------*/
    float lumaNW = FxaaTexOff(tex, pos.xy, ivec2(-1,-1), rcpFrame.xy).w;
    float lumaNE = FxaaTexOff(tex, pos.xy, ivec2( 1,-1), rcpFrame.xy).w;
    float lumaSW = FxaaTexOff(tex, pos.xy, ivec2(-1, 1), rcpFrame.xy).w;
    float lumaSE = FxaaTexOff(tex, pos.xy, ivec2( 1, 1), rcpFrame.xy).w;

	/*--------------------------------------------------------------------------*/
    #define FXAA_QUALITY__SUBPIX_TRIM_SCALE  (1.0/(1.0 - FXAA_QUALITY__SUBPIX_TRIM))
	/*--------------------------------------------------------------------------*/

    float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;
    float rangeL = abs(lumaL - lumaM);
    float blendL = FxaaSat((rangeL / range) - FXAA_QUALITY__SUBPIX_TRIM) * FXAA_QUALITY__SUBPIX_TRIM_SCALE; 
    blendL = min(FXAA_QUALITY__SUBPIX_CAP, blendL);
	
	/*--------------------------------------------------------------------------*/
    float edgeVert = 
              abs(lumaNW + (-2.0 * lumaN) + lumaNE) +
        2.0 * abs(lumaW  + (-2.0 * lumaM) + lumaE ) +
              abs(lumaSW + (-2.0 * lumaS) + lumaSE);
    float edgeHorz = 
              abs(lumaNW + (-2.0 * lumaW) + lumaSW) +
        2.0 * abs(lumaN  + (-2.0 * lumaM) + lumaS ) +
              abs(lumaNE + (-2.0 * lumaE) + lumaSE);
    bool horzSpan = edgeHorz >= edgeVert;
	/*--------------------------------------------------------------------------*/
    float lengthSign = horzSpan ? -rcpFrame.y : -rcpFrame.x;

    if(!horzSpan) lumaN = lumaW;
    if(!horzSpan) lumaS = lumaE;
    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);
    lumaN = (lumaN + lumaM) * 0.5;
    lumaS = (lumaS + lumaM) * 0.5;
	
	/*--------------------------------------------------------------------------*/
    bool pairN = gradientN >= gradientS;
    if(!pairN) lumaN = lumaS;
    if(!pairN) gradientN = gradientS;
    if(!pairN) lengthSign *= -1.0;
    vec2 posN;
    posN.x = pos.x + (horzSpan ? 0.0 : lengthSign * 0.5);
    posN.y = pos.y + (horzSpan ? lengthSign * 0.5 : 0.0);
	
	/*--------------------------------------------------------------------------*/
    #define FXAA_SEARCH_STEPS     6
    #define FXAA_SEARCH_THRESHOLD (1.0/4.0)
	/*--------------------------------------------------------------------------*/
    gradientN *= FXAA_SEARCH_THRESHOLD;
	/*--------------------------------------------------------------------------*/
    vec2 posP = posN;
    vec2 offNP = horzSpan ? vec2(rcpFrame.x, 0.0) : vec2(0.0f, rcpFrame.y); 
    float lumaEndN;
    float lumaEndP;
    bool doneN = false;
    bool doneP = false;
    posN += offNP * (-1.5);
    posP += offNP * ( 1.5);

    for(int i = 0; i < FXAA_SEARCH_STEPS; i++)
	{
        lumaEndN = FxaaTexTop(tex, posN.xy).w;
        lumaEndP = FxaaTexTop(tex, posP.xy).w;
        bool doneN2 = abs(lumaEndN - lumaN) >= gradientN;
        bool doneP2 = abs(lumaEndP - lumaN) >= gradientN;
        if(doneN2 && !doneN) posN += offNP;
        if(doneP2 && !doneP) posP -= offNP;
        if(doneN2 && doneP2) break;
        doneN = doneN2;
        doneP = doneP2;
        if(!doneN) posN -= offNP * 2.0;
        if(!doneP) posP += offNP * 2.0;
	}
	/*--------------------------------------------------------------------------*/
    float dstN = horzSpan ? pos.x - posN.x : pos.y - posN.y;
    float dstP = horzSpan ? posP.x - pos.x : posP.y - pos.y;
	
	/*--------------------------------------------------------------------------*/
    bool directionN = dstN < dstP;
    lumaEndN = directionN ? lumaEndN : lumaEndP;
	
	/*--------------------------------------------------------------------------*/
    if( ( ( lumaM - lumaN ) < 0.0 ) == ( ( lumaEndN - lumaN ) < 0.0 ) )
	{
        lengthSign = 0.0;
	}
	/*--------------------------------------------------------------------------*/
    float spanLength = (dstP + dstN);
    dstN = directionN ? dstN : dstP;
    float subPixelOffset = 0.5 + (dstN * (-1.0/(spanLength)));
    subPixelOffset += blendL * (1.0/8.0);
    subPixelOffset *= lengthSign;
    vec3 rgbF = FxaaTexTop(tex, vec2( pos.x + ( horzSpan ? 0.0 : subPixelOffset ), pos.y + ( horzSpan ? subPixelOffset : 0.0 ) ) ).xyz;
	/*--------------------------------------------------------------------------*/

    float lumaF = dot(rgbF, vec3(0.299, 0.587, 0.114)) + (1.0/(65536.0*256.0));
    float lumaB = mix(lumaF, lumaL, blendL);
    float scale = min(4.0, lumaB/lumaF);
    rgbF *= scale;
    return rgbF;
}

void main()
{
	vec3 vColor;
	if( iDebug == 0 )
	{
		vColor = FxaaPixelShader( gl_TexCoord[0].xy, sTexture, vInvViewport );
	}
	else
	{
		vColor = texture2D( sTexture, gl_TexCoord[0].xy ).rgb;
	}
	
	vec3 vBloom = texture2D( sBloom, gl_TexCoord[0].xy ).rgb;
	vColor += fGlowMultiplier * vBloom;

	gl_FragColor = vec4( texture3D( sLUT, vColor ).rgb, texture2D( sBlurData, gl_TexCoord[0].xy ).x );
}