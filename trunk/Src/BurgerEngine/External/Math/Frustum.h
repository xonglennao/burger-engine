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
#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "Vector.h"

#define FRUSTUM_LEFT   0
#define FRUSTUM_RIGHT  1
#define FRUSTUM_TOP    2
#define FRUSTUM_BOTTOM 3
#define FRUSTUM_FAR    4
#define FRUSTUM_NEAR   5

struct Plane {
	Plane(){}
	Plane(const float x, const float y, const float z, const float o){
		normal = vec3(x, y, z);
		float invLen = 1.0f / length(normal);
		normal *= invLen;
		offset = o * invLen;
	}

	float dist(const vec3 &pos) const {
		return dot(normal, pos) + offset;
	}

	vec3 normal;
	float offset;
};

class Frustum {
public:
    void loadFrustum(const mat4 &mvp);

    bool pointInFrustum(const vec3 &pos) const;
    bool allPointsInFrustum(const vec3* points, int numPoints) const;
    bool somePointsInFrustum(const vec3* points, int numPoints) const;
    bool sphereInFrustum(const vec3 &pos, const float radius) const;
    bool cubeInFrustum(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ) const;

	const Plane &getPlane(const int plane) const { return planes[plane]; }

protected:
	Plane planes[6];
};


#endif // _FRUSTUM_H_
