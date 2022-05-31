#include <Windows.h>
#include <thread>
#include <chrono>

/*
To do:
scroll
flip-phone-9-keys keyboard
maybe the sound board?
GUI
Funny idea that is probably impossible: when starting the program the user's mouse will move to where the mouse in the exe icon at
*/

void InputLoop()
{
	float m = 1;
	float wM = 1;

	float acceleration = 1.0f * m;
	float wAcceleration = 3.0f * wM;

	float maxSpeed = 15.0f * m;
	float wMaxSpeed = 25.0f * wM;

	float brakeSpeed = 1.0f * m;
	float brakeSpeedSingleAxis = 1.0f * m;
	float wBrakeSpeed = 3.0f * wM;
	
	float xSpeed = 0.0f;
	float ySpeed = 0.0f;
	float wSpeed = 0.0f;
	
	int sizeofinput = sizeof(INPUT);
	int mcrosecsPerTick = 10000;
	
	bool lButton = false;
	bool rButton = false;
	bool mButton = false;
	bool exit = false;

	INPUT mouseMove;
	
	INPUT mouseLDown;
	INPUT mouseLUp;
	
	INPUT mouseRDown;
	INPUT mouseRUp;
	
	INPUT mouseMidDown;
	INPUT mouseMidUp;

	INPUT mouseWheel;

	mouseMove.type = INPUT_MOUSE;
	mouseMove.mi.time = 0;
	mouseMove.mi.dwFlags = MOUSEEVENTF_MOVE;

	mouseLDown.type = INPUT_MOUSE;
	mouseLDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	mouseLDown.mi.time = 0;
	
	mouseLUp.type = INPUT_MOUSE;
	mouseLUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	mouseLUp.mi.time = 0;

	mouseRDown.type = INPUT_MOUSE;
	mouseRDown.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	mouseRDown.mi.time = 0;

	mouseRUp.type = INPUT_MOUSE;
	mouseRUp.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	mouseRUp.mi.time = 0;

	mouseMidDown.type = INPUT_MOUSE;
	mouseMidDown.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
	mouseMidDown.mi.time = 0;

	mouseMidUp.type = INPUT_MOUSE;
	mouseMidUp.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
	mouseMidUp.mi.time = 0;

	mouseWheel.type = INPUT_MOUSE;
	mouseWheel.mi.dwFlags = MOUSEEVENTF_WHEEL;
	mouseWheel.mi.time = 0;

	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD1);
	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD2);
	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD3);
	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD4);
	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD5);
	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD6);
	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD7);
	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD8);
	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD9);
	RegisterHotKey(NULL, 0, MOD_NOREPEAT, VK_NUMPAD0);
	
	bool movedY = false;
	bool movedX = false;
	bool movedW = false;

	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	auto exitStart = std::chrono::high_resolution_clock::now();
	auto exitEnd = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	
	while (true)
	{
		start = std::chrono::high_resolution_clock::now();

		movedY = false;
		movedX = false;

		// down
		if (GetAsyncKeyState(VK_NUMPAD5))
		{
			movedY = true;
			if (ySpeed < maxSpeed)
				ySpeed += acceleration;
			if (ySpeed > maxSpeed)
				ySpeed = maxSpeed;
		}
		// up
		if (GetAsyncKeyState(VK_NUMPAD8))
		{
			movedY = true;
			if (ySpeed > -maxSpeed)
				ySpeed -= acceleration;
			if (ySpeed < -maxSpeed)
				ySpeed = -maxSpeed;
		}
		// right
		if (GetAsyncKeyState(VK_NUMPAD6))
		{
			movedX = true;
			if (xSpeed < maxSpeed)
				xSpeed += acceleration;
			if (xSpeed > maxSpeed)
				xSpeed = maxSpeed;
		}
		// left
		if (GetAsyncKeyState(VK_NUMPAD4))
		{
			movedX = true;
			if (xSpeed > -maxSpeed)
				xSpeed -= acceleration;
			if (xSpeed < -maxSpeed)
				xSpeed = -maxSpeed;
		}
		// mouseL
		if (GetAsyncKeyState(VK_NUMPAD7))
		{
			if (!lButton)
				SendInput(1, &mouseLDown, sizeofinput);
			lButton = true;
		}
		else
		{
			if (lButton)
				SendInput(1, &mouseLUp, sizeofinput);
			lButton = false;
		}
		// mouseR
		if (GetAsyncKeyState(VK_NUMPAD9))
		{
			if (!rButton)
				SendInput(1, &mouseRDown, sizeofinput);
			rButton = true;
		}
		else
		{
			if (rButton)
				SendInput(1, &mouseRUp, sizeofinput);
			rButton = false;
		}
		// mouseM
		if (GetAsyncKeyState(VK_NUMPAD2))
		{
			if (!mButton)
				SendInput(1, &mouseMidDown, sizeofinput);
			mButton = true;
		}
		else
		{
			if (mButton)
				SendInput(1, &mouseMidUp, sizeofinput);
			mButton = false;
		}
		// exit
		if (GetAsyncKeyState(VK_END))
		{
			if (exit == false)
			{
				exitStart = std::chrono::high_resolution_clock::now();
				exit = true;
			}
			else
			{
				exitEnd = std::chrono::high_resolution_clock::now();
				duration = std::chrono::duration_cast<std::chrono::microseconds>(exitEnd - exitStart);

				if (duration.count() > 5000000)
					return;
			}
		}
		else
		{
			exit = false;
		}

		// wUp
		if (GetAsyncKeyState(VK_NUMPAD1))
		{
			if (wSpeed < wMaxSpeed)
				wSpeed += wAcceleration;
			if (wSpeed > wMaxSpeed)
				wSpeed = wMaxSpeed;
			movedW = true;
		}
		else
		{
			movedW = false;
		}
		// wDown
		if (GetAsyncKeyState(VK_NUMPAD0))
		{
			if (wSpeed > -wMaxSpeed)
				wSpeed -= wAcceleration;
			if (wSpeed < -wMaxSpeed)
				wSpeed = -wMaxSpeed;
			movedW = true;
		}
		else if (!movedW) // lmao what a waste of time that was
		{
			movedW = false;
		}
		
		if (!movedX)
		{
			if (movedY)
			{
				if (xSpeed > brakeSpeedSingleAxis)
					xSpeed -= brakeSpeedSingleAxis;
				else if (xSpeed < -brakeSpeedSingleAxis)
					xSpeed += brakeSpeedSingleAxis;
				else
					xSpeed = 0;
			}
			else
			{
				if (xSpeed > brakeSpeed)
					xSpeed -= brakeSpeed;
				else if (xSpeed < -brakeSpeed)
					xSpeed += brakeSpeed;
				else
					xSpeed = 0;
			}
		}
		if (!movedY)
		{
			if (movedX)
			{
				if (ySpeed > brakeSpeedSingleAxis)
					ySpeed -= brakeSpeedSingleAxis;
				else if (ySpeed < -brakeSpeedSingleAxis)
					ySpeed += brakeSpeedSingleAxis;
				else
					ySpeed = 0;
			}
			else
			{
				if (ySpeed > brakeSpeed)
					ySpeed -= brakeSpeed;
				else if (ySpeed < -brakeSpeed)
					ySpeed += brakeSpeed;
				else
					ySpeed = 0;
			}
		}
		if (!movedW)
		{
			if (wSpeed > wBrakeSpeed)
				wSpeed -= wBrakeSpeed;
			else if (wSpeed < -wBrakeSpeed)
				wSpeed += wBrakeSpeed;
			else
				wSpeed = 0;
		}

		mouseMove.mi.dx = (long)xSpeed;
		mouseMove.mi.dy = (long)ySpeed;
		SendInput(1, &mouseMove, sizeofinput);
		mouseWheel.mi.mouseData = (unsigned long)(wSpeed);
		SendInput(1, &mouseWheel, sizeofinput);
		
		end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::this_thread::sleep_for(std::chrono::microseconds(mcrosecsPerTick - duration.count()));
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	InputLoop();
}