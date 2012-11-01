#include "AbstractComponent.h"
#include "CompositeComponent.h"


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
const vec3 AbstractComponent::GetPos() const
{
	if ( m_pParentNode )
	{
		vec3 f3ParentRotation = m_pParentNode->GetRotation();
		vec3 f3Pos = m_f3Position * m_pParentNode->GetScale();
		vec4 f4NewPos = rotateY( f3ParentRotation.y * DEG_TO_RAD ) * vec4(f3Pos,1.0f);
		return f4NewPos.xyz() + m_pParentNode->GetPos();
	}
	else
	{
		return m_f3Position;
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
float AbstractComponent::GetScale() const
{
	if ( m_pParentNode )
	{
		return m_fScale * m_pParentNode->GetScale();
	}
	else
	{
		return m_fScale;
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------	
const vec3 AbstractComponent::GetRotation() const
{
	if ( m_pParentNode )
	{
		return m_f3Rotation + m_pParentNode->GetRotation();
	}
	else
	{
		return m_f3Rotation;
	}
}
