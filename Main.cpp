#include <Windows.h>
#include <thread>
#include <chrono>

void InputLoop()
{
	float maxSpeed = 10.0f;
	float acceleration = 0.5f;
	float brakeSpeed = 1.0f;
	float brakeSpeedSingleAxis = 0.5f;
	float xSpeed = 0.0f;
	float ySpeed = 0.0f;
	int xDes = 0;
	int yDes = 0;

	int sizeofinput = sizeof(INPUT);
	
	bool lButton = false;
	bool rButton = false;
	bool mButton = false;

	INPUT inputsDown[3];
	INPUT inputsUp[3];

	inputsDown[0].type = INPUT_MOUSE;
	inputsDown[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	inputsDown[1].type = INPUT_MOUSE;
	inputsDown[1].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

	inputsDown[2].type = INPUT_MOUSE;
	inputsDown[2].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;

	inputsUp[0].type = INPUT_MOUSE;
	inputsUp[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	inputsUp[1].type = INPUT_MOUSE;
	inputsUp[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	
	inputsUp[2].type = INPUT_MOUSE;
	inputsUp[2].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;

	inputsUp[0].mi.time = 0;
	inputsUp[1].mi.time = 0;
	inputsUp[2].mi.time = 0;
	inputsDown[0].mi.time = 0;
	inputsDown[1].mi.time = 0;
	inputsDown[2].mi.time = 0;

	RegisterHotKey(NULL, 1, MOD_NOREPEAT, VK_NUMPAD5);
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, VK_NUMPAD8);
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, VK_NUMPAD6);
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, VK_NUMPAD4);
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, VK_NUMPAD7);
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, VK_NUMPAD9);
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, VK_NUMPAD2);

	POINT MousePos;
	
	bool movedY;
	bool movedX;

	while (true)
	{
		auto start = std::chrono::high_resolution_clock::now();
		
		movedY = false;
		movedX = false;

		// down
		if (GetAsyncKeyState(VK_NUMPAD5))
		{
			movedY = true;
			if (ySpeed < maxSpeed)
				ySpeed += acceleration;
		}
		// up
		if (GetAsyncKeyState(VK_NUMPAD8))
		{
			movedY = true;
			if (ySpeed > -maxSpeed)
				ySpeed -= acceleration;
		}
		// right
		if (GetAsyncKeyState(VK_NUMPAD6))
		{
			movedX = true;
			if (xSpeed < maxSpeed)
				xSpeed += acceleration;
		}
		// left
		if (GetAsyncKeyState(VK_NUMPAD4))
		{
			movedX = true;
			if (xSpeed > -maxSpeed)
				xSpeed -= acceleration;
		}
		// lButton
		if (GetAsyncKeyState(VK_NUMPAD7))
		{
			if (!lButton)
				SendInput(1, &inputsDown[0], sizeofinput);
			lButton = true;
		}
		else
		{
			if (lButton)
				SendInput(1, &inputsUp[0], sizeofinput);
			lButton = false;
		}
		// rButton
		if (GetAsyncKeyState(VK_NUMPAD9))
		{
			if (!rButton)
				SendInput(1, &inputsDown[1], sizeofinput);
			rButton = true;
		}
		else
		{
			if (rButton)
				SendInput(1, &inputsUp[1], sizeofinput);
			rButton = false;
		}
		// mButton
		if (GetAsyncKeyState(VK_NUMPAD2))
		{
			if (!mButton)
				SendInput(1, &inputsDown[2], sizeofinput);
			mButton = true;
		}
		else
		{
			if (mButton)
				SendInput(1, &inputsUp[2], sizeofinput);
			mButton = false;
		}
		// scroll up

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
		GetCursorPos(&MousePos);
		SetCursorPos(MousePos.x + (int)xSpeed, MousePos.y + (int)ySpeed);
		GetCursorPos(&MousePos);
		
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);;
		std::this_thread::sleep_for(std::chrono::microseconds(10000 - duration.count()));
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::thread t_inputmanager(InputLoop);

	while (true) {}

	t_inputmanager.detach();
}