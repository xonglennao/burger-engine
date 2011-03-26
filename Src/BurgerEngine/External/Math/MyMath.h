/* 
Render framework Copyright (c) 2001-2010 Emil Persson. Changes by Jack 
Hoxley, Ralf Kornmann, Niko Suni, Jason Zink and Wolfgang Engel

http://code.google.com/p/??/  

This software is provided 'as-is', without any express or implied warranty. 
In no event will the authors be held liable for any damages arising from the 
use of this software. Permission is granted to anyone to use this software 
for any purpose,  including commercial applications, and to alter it and 
redistribute it freely,  subject to the following restrictions:  

1. The origin of this software must not be misrepresented; you must not claim 
that you wrote the original software. If you use this software in a product, 
an acknowledgment in the product documentation would be appreciated but is not 
required. 

2. Altered source versions must be plainly marked as such, and must not be 
misrepresented as being the original software. 

3. This notice may not be removed or altered from any source distribution. 
*/ 
#ifndef _MYMATH_H_
#define _MYMATH_H_

#include <math.h>

#define PI 3.14159265358979323846f
#define DEG_TO_RAD PI / 180.0f
#define RAD_TO_DEG 180.f / PI 

// This code supposedly originates from Id-software
// It computes a fast 1 / sqrtf(v) approximation
/*
inline float rsqrtf(float v){
    float v_half = v * 0.5f;
    int i = *(int *) &v;
    i = 0x5f3759df - (i >> 1);
    v = *(float *) &i;
    return v * (1.5f - v_half * v * v);
}
*/
inline float rsqrtf(const float v){
	union {
		float vh;
		int i0;
	};

	union {
		float vr;
		int i1;
	};

	vh = v * 0.5f;
	i1 = 0x5f3759df - (i0 >> 1);
	return vr * (1.5f - vh * vr * vr);
}


inline float sqrf(const float x){
    return x * x;
}

inline float sincf(const float x){
	return (x == 0)? 1 : sinf(x) / x;
}

#define roundf(x) floorf((x) + 0.5f)

#ifndef min
#define min(x, y) ((x < y)? x : y)
#endif

#ifndef max
#define max(x, y) ((x > y)? x : y)
#endif

inline float intAdjustf(const float x, const float diff = 0.01f){
	float f = roundf(x);

	return (fabsf(f - x) < diff)? f : x;
}

inline unsigned int getClosestPowerOfTwo(const unsigned int x){
	unsigned int i = 1;
	while (i < x) i += i;

	if (4 * x < 3 * i) i >>= 1;
	return i;
}

inline unsigned int getUpperPowerOfTwo(const unsigned int x){
	unsigned int i = 1;
	while (i < x) i += i;

	return i;
}

inline unsigned int getLowerPowerOfTwo(const unsigned int x){
	unsigned int i = 1;
	while (i <= x) i += i;

	return i >> 1;
}

inline int round(float x){
	if (x > 0){
		return int(x + 0.5f);
	} else {
		return int(x - 0.5f);
	}
}

#endif // _MYMATH_H_
