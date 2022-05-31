#include <Windows.h>
#include <thread>
#include <chrono>

/*
To do:
flip-phone-9-keys keyboard
scroll
maybe the sound board?
*/

void InputLoop()
{
	float m = 1;
	float maxSpeed = 15.0f * m;
	float acceleration = 1.0f * m;
	float brakeSpeed = 1.0f * m;
	float brakeSpeedSingleAxis = 1.0f * m;
	float xSpeed = 0.0f;
	float ySpeed = 0.0f;
	int xDes = 0;
	int yDes = 0;

	int sizeofinput = sizeof(INPUT);
	int microsecondPerTick = 10000;
	
	bool lButton = false;
	bool rButton = false;
	bool mButton = false;
	bool exit = false;

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

	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);;
	auto exitStart = std::chrono::high_resolution_clock::now();
	auto exitEnd = std::chrono::high_resolution_clock::now();

	int timer = 500;

	INPUT mouseInput;
	mouseInput.type = INPUT_MOUSE;
	mouseInput.mi.time = 0;
	mouseInput.mi.dwFlags = MOUSEEVENTF_MOVE;
	
	while (true)
	{
		start = std::chrono::high_resolution_clock::now();
		
		timer--;

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

		if (xSpeed > maxSpeed)
			xSpeed = maxSpeed;
		if (xSpeed < -maxSpeed)
			xSpeed = -maxSpeed;

		if (ySpeed > maxSpeed)
			ySpeed = maxSpeed;
		if (ySpeed < -maxSpeed)
			ySpeed = -maxSpeed;

		/*
		GetCursorPos(&MousePos);
		SetCursorPos(MousePos.x + (int)xSpeed, MousePos.y + (int)ySpeed);
		GetCursorPos(&MousePos);
		/*/
		mouseInput.mi.dx = (int)xSpeed;
		mouseInput.mi.dy = (int)ySpeed;
		SendInput(1, &mouseInput, sizeofinput);
		//*/
		end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::this_thread::sleep_for(std::chrono::microseconds(microsecondPerTick - duration.count()));
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	InputLoop();
}