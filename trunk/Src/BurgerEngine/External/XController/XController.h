#ifndef _XCONTROLLER_H_
#define _XCONTROLLER_H_


class XController_InputState;
class XController_VibrationState;


class XController
{
public:

	class StickState
	{
	public:
		float dirX;
		float dirY;
		StickState(float dirX, float dirY);
	};

	static int* getConnectedPlayers(int* numPlayers);

	typedef unsigned short ButtonEnum;
	static const ButtonEnum DPAD_UP = 0x0001;
	static const ButtonEnum DPAD_DOWN = 0x0002;
	static const ButtonEnum DPAD_LEFT = 0x0004;
	static const ButtonEnum DPAD_RIGHT = 0x0008;
	static const ButtonEnum START = 0x0010;
	static const ButtonEnum BACK = 0x0020;
	static const ButtonEnum LEFT_THUMB = 0x0040;
	static const ButtonEnum RIGHT_THUMB = 0x0080;
	static const ButtonEnum LEFT_SHOULDER = 0x0100;
	static const ButtonEnum RIGHT_SHOULDER = 0x0200;
	static const ButtonEnum A = 0x1000;
	static const ButtonEnum B = 0x2000;
	static const ButtonEnum X = 0x4000;
	static const ButtonEnum Y = 0x8000;

	XController(int playerIndex);
	~XController();

	void update();  // Call this each frame
	bool isPressed(ButtonEnum button);
	StickState getLeftStickState();
	StickState getRightStickState();
	StickState getStickState(float fX, float fY, float fDeadZone);
	StickState getDPadStickState();
	float getLeftTriggerState();
	float getRightTriggerState();
	bool isConnected();
	int getControllerIndex() const;

	void vibrate(float leftVal, float rightVal);
	void vibrateOff();
	void vibrateLeft(float leftVal);
	void vibrateRight(float rightVal);


private:

	XController_InputState* lastState;
	XController_InputState* currentState;
	XController_VibrationState* Vibration;

	int controllerNum;
	bool connected;
	int triggerThreshold;

};

#endif
