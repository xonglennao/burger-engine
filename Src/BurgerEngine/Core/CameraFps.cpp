#include "CameraFps.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Input/EventManager.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CameraFps::Initialize( const unsigned int a_iWindowWidth, const unsigned int a_iWindowHeight )
{
	//AbstractCamera::Initialize();

	//Register callback
	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&CameraFps::OnDownKey));
	Engine::GrabInstance().GrabEventManager().RegisterCallbackMousePassiveMotion(
		EventManager::CallbackMouseMotion(this,&CameraFps::OnMouseMoved));
	Engine::GrabInstance().GrabEventManager().RegisterCallbackResize(
		EventManager::CallbackResize(this,&CameraFps::OnResize));

	/// \todo Get back init parameters
	//m_f2WindowSize.set( static_cast<float>(a_iWindowWidth), static_cast<float>(a_iWindowHeight) );
	m_f2WindowSize = vec2( static_cast<float>(a_iWindowWidth), static_cast<float>(a_iWindowHeight) );
	m_fMovingSpeed = 0.02f;

	m_fMouseSpeed = 0.1f;

	m_fAlpha=0;
	m_fPhi=-90;

	m_bForward = false;
	m_bBackward = false;
	m_bLeft = false;
	m_bRight = false;

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CameraFps::Terminate()
{
	// Unregister callback
	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&CameraFps::OnDownKey));
	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackMouseActiveMotion(
		EventManager::CallbackMouseMotion(this,&CameraFps::OnMouseMoved));
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CameraFps::Update()
{
	vec3& rf3Pos = _GrabPos();
	
	if( m_bForward )
	{
		rf3Pos.x += m_fMovingSpeed * m_f3Direction.x;
		rf3Pos.y += m_fMovingSpeed * m_f3Direction.y;
		rf3Pos.z += m_fMovingSpeed * m_f3Direction.z;
	}
	if( m_bBackward )
	{
		rf3Pos.x -= m_fMovingSpeed * m_f3Direction.x;
		rf3Pos.y -= m_fMovingSpeed * m_f3Direction.y;
		rf3Pos.z -= m_fMovingSpeed * m_f3Direction.z;	
	}
	if( m_bLeft )
	{
		rf3Pos.x += m_fMovingSpeed * m_f3Right.x;
		rf3Pos.z += m_fMovingSpeed * m_f3Right.z;	
	}
	if( m_bRight )
	{
		rf3Pos.x -= m_fMovingSpeed * m_f3Right.x;
		rf3Pos.z -= m_fMovingSpeed * m_f3Right.z;
	}

	_InternalUpdate();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool CameraFps::OnDownKey(unsigned char a_cKey)
{
	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool CameraFps::OnResize(unsigned int a_uWidth, unsigned int a_uHeight)
{
	m_f2WindowSize = vec2( static_cast<float>(a_uWidth),static_cast<float>(a_uHeight) );
	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool CameraFps::OnMouseMoved(unsigned int a_uX, unsigned int a_uY)
{

	m_fAlpha += (m_f2WindowSize.x / 2.0f - static_cast<float>(a_uX) )* m_fMouseSpeed;
	m_fPhi +=(m_f2WindowSize.y /2.0f - static_cast<float>(a_uY) )*m_fMouseSpeed;

	if(m_fPhi>-1)
	{
		m_fPhi=-1;
	}
	if(m_fPhi<-179)
	{
		m_fPhi=-179;
	}

	if(m_fAlpha>360 || m_fAlpha < -360)
	{
		m_fAlpha = 0.0; 
	}
	_InternalUpdate();

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CameraFps::_InternalUpdate()
{
	vec3& rf3Direction = _GrabDirection();

	rf3Direction.x = sinf(m_fPhi*M_PI/180.0f)*sin(m_fAlpha*M_PI/180.0f);
	rf3Direction.y = cosf(m_fPhi*M_PI/180.0f);
	rf3Direction.z = sinf(m_fPhi*M_PI/180.0f)*cos(m_fAlpha*M_PI/180.0f);

	//Cross product
	vec3& rf3Up = _GrabUp();
	m_f3Right = cross( rf3Up, rf3Direction);

	//normalize
	m_f3Right = normalize( m_f3Right );

	vec3& rf3Aim = _GrabAim();
	vec3& rf3Pos = _GrabPos();

	rf3Aim = rf3Pos + rf3Direction;
}

void CameraFps::SetFlag( CameraFlagEnum eFlag, bool bValue )
{
	switch( eFlag )
	{
	case E_CAMERA_FORWARD:
		m_bForward = bValue;
		break;
	case E_CAMERA_BACKWARD:
		m_bBackward = bValue;	
		break;
	case E_CAMERA_LEFT:
		m_bLeft = bValue;
		break;
	case E_CAMERA_RIGHT:
		m_bRight = bValue;
		break;
	}
}

