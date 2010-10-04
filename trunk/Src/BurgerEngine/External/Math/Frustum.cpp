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

//edited by Baptiste Malaga

#include "Frustum.h"
#include <iostream>
void Frustum::loadFrustum(const mat4 &mvp)
{
	planes[FRUSTUM_LEFT  ] = Plane(mvp.rows[0].w + mvp.rows[0].x, mvp.rows[1].w + mvp.rows[1].x, mvp.rows[2].w + mvp.rows[2].x,  mvp.rows[3].w + mvp.rows[3].x);
	planes[FRUSTUM_RIGHT ] = Plane(mvp.rows[0].w - mvp.rows[0].x, mvp.rows[1].w - mvp.rows[1].x, mvp.rows[2].w - mvp.rows[2].x,  mvp.rows[3].w - mvp.rows[3].x);

	planes[FRUSTUM_TOP   ] = Plane(mvp.rows[0].w - mvp.rows[0].y, mvp.rows[1].w - mvp.rows[1].y, mvp.rows[2].w - mvp.rows[2].y,  mvp.rows[3].w - mvp.rows[3].y);
	planes[FRUSTUM_BOTTOM] = Plane(mvp.rows[0].w + mvp.rows[0].y, mvp.rows[1].w + mvp.rows[1].y, mvp.rows[2].w + mvp.rows[2].y,  mvp.rows[3].w + mvp.rows[3].y);

#if 0 // Direct3D
	planes[FRUSTUM_FAR   ] = Plane(mvp.rows[0].w - mvp.rows[0].z, mvp.rows[1].w - mvp.rows[1].z, mvp.rows[2].w - mvp.rows[2].z,  mvp.rows[3].w - mvp.rows[3].z);
	planes[FRUSTUM_NEAR  ] = Plane(mvp.rows[0].z, mvp.rows[1].z, mvp.rows[2].z, mvp.rows[3].z);
#else
	planes[FRUSTUM_FAR   ] = Plane(mvp.rows[0].w - mvp.rows[0].z, mvp.rows[1].w - mvp.rows[1].z, mvp.rows[2].w - mvp.rows[2].z,  mvp.rows[3].w - mvp.rows[3].z);
	planes[FRUSTUM_NEAR  ] = Plane(mvp.rows[0].w + mvp.rows[0].z, mvp.rows[1].w + mvp.rows[1].z, mvp.rows[2].w + mvp.rows[2].z,  mvp.rows[3].w + mvp.rows[3].z);
#endif

}

bool Frustum::pointInFrustum(const vec3 &pos) const {
	for (int i = 0; i < 6; i++){
		if (planes[i].dist(pos) <= 0) return false;
	}
    return true;
}

// returns true if all points are inside of the frustum
bool Frustum::allPointsInFrustum(const vec3* points, int numPoints) const
{
	for (int i = 0; i < numPoints; i++)
	{
		if (!pointInFrustum(points[i]))
			return false;
	}
	return true;
}

// returns true if any points are inside of the frustum
bool Frustum::somePointsInFrustum(const vec3* points, int numPoints) const
{
	for (int i = 0; i < numPoints; i++)
	{
		if (pointInFrustum(points[i]))
			return true;
	}
	return false;
}

bool Frustum::sphereInFrustum(const vec3 &pos, const float radius) const {
	for (int i = 0; i < 6; i++){
		float fDist = planes[i].dist(pos);
		//if( i== 4 ) std::cout << fDist << std::endl;
		if (planes[i].dist(pos) <= -radius) return false;
	}
    return true;
}

bool Frustum::cubeInFrustum(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ) const {
    for (int i = 0; i < 6; i++){
		if (planes[i].dist(vec3(minX, minY, minZ)) > 0) continue;
		if (planes[i].dist(vec3(minX, minY, maxZ)) > 0) continue;
		if (planes[i].dist(vec3(minX, maxY, minZ)) > 0) continue;
		if (planes[i].dist(vec3(minX, maxY, maxZ)) > 0) continue;
		if (planes[i].dist(vec3(maxX, minY, minZ)) > 0) continue;
		if (planes[i].dist(vec3(maxX, minY, maxZ)) > 0) continue;
		if (planes[i].dist(vec3(maxX, maxY, minZ)) > 0) continue;
		if (planes[i].dist(vec3(maxX, maxY, maxZ)) > 0) continue;
        return false;
    }
    return true;
}
