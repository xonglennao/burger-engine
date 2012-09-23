#include "XController.h"
#include <cmath>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "XInput.h"


#define VIBRATION_MAX 65535
const float MAX_STICK_VALUE = 0.000030f;

class XController_InputState
{
public:
	XINPUT_STATE state;
};

class XController_VibrationState
{
public:
	XINPUT_VIBRATION vibration;
};

static inline void clamp(float& f, float low, float high)
{
	if(f < low)
		f = low;
	else if(f > high)
		f = high;
}


int* XController::getConnectedPlayers(int* numPlayers)
{
	int* s = new int[4];
	for(int i = 0; i < 4; i++)
		s[i] = -1;

	int num = 0;
	for(int i = 0; i < 4; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Get the state of the controller from XInput.
		if(XInputGetState(i, &state) == ERROR_SUCCESS)
		{
			s[num] = i;
			num++;
		}
	}

	if(numPlayers != NULL)
		*numPlayers = num;
	return s;
}


XController::StickState::StickState(float dirX, float dirY)
	: dirX(dirX), dirY(dirY)
{}



XController::XController(int playerIndex)
	: controllerNum(playerIndex), connected(false)
	, triggerThreshold(XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
{
	lastState = new XController_InputState;
	currentState = new XController_InputState;
	Vibration = new XController_VibrationState;

	ZeroMemory(&lastState->state, sizeof(XINPUT_STATE));
	ZeroMemory(&currentState->state, sizeof(XINPUT_STATE));
	ZeroMemory(&Vibration->vibration, sizeof(XINPUT_VIBRATION));
}

XController::~XController()
{
	vibrateOff();

	delete lastState;
	delete currentState;
	delete Vibration;
}

void XController::update()
{
	// Store the old state
	lastState->state = currentState->state;

	// Get the new state
	ZeroMemory(&currentState->state, sizeof(XINPUT_STATE));
	connected = (XInputGetState(controllerNum, &currentState->state) == ERROR_SUCCESS);
}

bool XController::isPressed(ButtonEnum button)
{
	return (currentState->state.Gamepad.wButtons & button);
}

bool XController::isConnected()
{
	return connected;
}

int XController::getControllerIndex() const
{
	return controllerNum;
}

XController::StickState XController::getLeftStickState()
{
	return getStickState(currentState->state.Gamepad.sThumbLX, currentState->state.Gamepad.sThumbLY,XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
}

XController::StickState XController::getRightStickState()
{
	return getStickState(currentState->state.Gamepad.sThumbRX, currentState->state.Gamepad.sThumbRY,XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

XController::StickState XController::getStickState(float fX, float fY, float fDeadZone)
{
	//determine how far the controller is pushed
	float magnitude = sqrt(fX*fX + fY*fY);

	//check if the controller is outside a circular dead zone
	if (magnitude > fDeadZone)
	{
		return StickState(fX*MAX_STICK_VALUE, fY*MAX_STICK_VALUE);
	}
	else
	{
		return StickState(0.0f, 0.0f);
	}
}

#define ONE_OVER_SQRT_2 0.707106781f

XController::StickState XController::getDPadStickState()
{
	float x = 0.0f;
	float y = 0.0f;

	bool up = isPressed(DPAD_UP);
	bool down = isPressed(DPAD_DOWN);
	bool left = isPressed(DPAD_LEFT);
	bool right = isPressed(DPAD_RIGHT);

	if(up)
	{
		if(left)
		{
			x = -ONE_OVER_SQRT_2;
			y = ONE_OVER_SQRT_2;
		}
		else if(right)
		{
			x = ONE_OVER_SQRT_2;
			y = ONE_OVER_SQRT_2;
		}
		else
			y = 1.0f;
	}
	else if(down)
	{
		if(left)
		{
			x = -ONE_OVER_SQRT_2;
			y = -ONE_OVER_SQRT_2;
		}
		else if(right)
		{
			x = ONE_OVER_SQRT_2;
			y = -ONE_OVER_SQRT_2;
		}
		else
			y = -1.0f;
	}
	else if(left)
	{
		x = -1.0f;
	}
	else if(right)
	{
		x = 1.0f;
	}

	return StickState(x, y);
}

float XController::getLeftTriggerState()
{
	float t = currentState->state.Gamepad.bLeftTrigger;
	if(t <= triggerThreshold)
		return 0.0f;
	return (t - triggerThreshold) / (255 - triggerThreshold);
}

float XController::getRightTriggerState()
{
	float t = currentState->state.Gamepad.bRightTrigger;
	if(t <= triggerThreshold)
		return 0.0f;
	return (t - triggerThreshold) / (255 - triggerThreshold);
}





void XController::vibrate(float leftVal, float rightVal)
{
	leftVal = fabs(leftVal);
	rightVal = fabs(rightVal);
	clamp(leftVal, 0.0f, 1.0f);
	clamp(rightVal, 0.0f, 1.0f);
	// Set the Vibration Values
	Vibration->vibration.wLeftMotorSpeed = leftVal*VIBRATION_MAX;
	Vibration->vibration.wRightMotorSpeed = rightVal*VIBRATION_MAX;

	// Vibrate the controller
	XInputSetState(controllerNum, &Vibration->vibration);
}

void XController::vibrateOff()
{
	// Set the Vibration Values
	Vibration->vibration.wLeftMotorSpeed = 0;
	Vibration->vibration.wRightMotorSpeed = 0;

	// Vibrate the controller
	XInputSetState(controllerNum, &Vibration->vibration);
}

void XController::vibrateLeft(float leftVal)
{
	leftVal = fabs(leftVal);
	clamp(leftVal, 0.0f, 1.0f);
	Vibration->vibration.wLeftMotorSpeed = leftVal*VIBRATION_MAX;

	// Vibrate the controller
	XInputSetState(controllerNum, &Vibration->vibration);
}

void XController::vibrateRight(float rightVal)
{
	rightVal = fabs(rightVal);
	clamp(rightVal, 0.0f, 1.0f);
	Vibration->vibration.wRightMotorSpeed = rightVal*VIBRATION_MAX;

	// Vibrate the controller
	XInputSetState(controllerNum, &Vibration->vibration);
}
