#include "SceneObject.h"

SceneObject::SceneObject()
	: m_f3Position( vec3(0.0f,0.0f,0.0f) )
	, m_fViewZ( 0.0f )
	, m_f3Rotation( vec3(0.0f,0.0f,0.0f) )
	, m_pBoundingBox( NULL )
{
}

SceneObject::~SceneObject()
{
};