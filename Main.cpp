#include <Windows.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <string>
#include <iostream>

/*
Future features:
flip-phone-9-keys keyboard
soundboard
HUD
*/

namespace Data
{
	bool active = true;
	int mode = 0;
	
	INPUT mouseMove;
	INPUT mouseWheel;
	INPUT mouseLDown;
	INPUT mouseLUp;
	INPUT mouseRDown;
	INPUT mouseRUp;
	INPUT mouseMidDown;
	INPUT mouseMidUp;

	int mouseMoveUpKey;
	int mouseMoveDownKey;
	int mouseMoveRightKey;
	int mouseMoveLeftKey;
	int mouseWheelUpKey;
	int mouseWheelDownKey;
	int mouseLeftKey;
	int mouseRightKey;
	int mouseMidKey;
	int functionKey;
	int exitKey;
	int activateKey;
	int modeUpKey;
	int modeDownKey;
	
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
	bool activating = false;
	bool exiting = false;
	bool modeUpButton = false;
	bool modeDownButton = false;
	bool movedY = false;
	bool movedX = false;
	bool movedW = false;

	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	auto exitStart = std::chrono::high_resolution_clock::now();
	auto exitEnd = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	
	void InitializeInputs()
	{
		mouseMove.type = INPUT_MOUSE;
		mouseMove.mi.time = 0;
		mouseMove.mi.dwFlags = MOUSEEVENTF_MOVE;

		mouseWheel.type = INPUT_MOUSE;
		mouseWheel.mi.dwFlags = MOUSEEVENTF_WHEEL;
		mouseWheel.mi.time = 0;

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
	}

	float ConvertValue(std::string value)
	{
		if (value.size() >= 2)
			if (value[0] == '0' && value[1] == 'x')
				return std::stoi(value, nullptr, 16);
		else if (value.size() > 0)
			return std::stof(value);
	}
	
	void SetValue(std::string name, std::string value)
	{
		if (name == "acceleration")
			acceleration = std::stof(value);
		
		else if (name == "wAcceleration")
			wAcceleration = std::stof(value);
		
		else if (name == "maxSpeed")
			maxSpeed = std::stof(value);
		
		else if (name == "wMaxSpeed")
			wMaxSpeed = std::stof(value);
		
		else if (name == "brakeSpeed")
			brakeSpeed = std::stof(value);
		
		else if (name == "brakeSpeedSingleAxis")
			brakeSpeedSingleAxis = std::stof(value);
		
		else if (name == "wBrakeSpeed")
			wBrakeSpeed = std::stof(value);
		
		else if (name == "m")
			m = std::stof(value);
		
		else if (name == "wM")
			wM = std::stof(value);


		
		else if (name == "mouseMoveUp")
			mouseMoveUpKey = ConvertValue(value);
		
		else if (name == "mouseMoveDown")
			mouseMoveDownKey = ConvertValue(value);
		
		else if (name == "mouseMoveLeft")
			mouseMoveLeftKey = ConvertValue(value);
		
		else if (name == "mouseMoveRight")
			mouseMoveRightKey = ConvertValue(value);
		
		else if (name == "mouseWheelUp")
			mouseWheelUpKey = ConvertValue(value);
		
		else if (name == "mouseWheelDown")
			mouseWheelDownKey = ConvertValue(value);
		
		else if (name == "mouseLeft")
			mouseLeftKey = ConvertValue(value);
		
		else if (name == "mouseRight")
			mouseRightKey = ConvertValue(value);
		
		else if (name == "mouseMid")
			mouseMidKey = ConvertValue(value);
		
		else if (name == "function")
			functionKey = ConvertValue(value);
		
		else if (name == "exit")
			exitKey = ConvertValue(value);
		
		else if (name == "activate")
			activateKey = ConvertValue(value);
		
		else if (name == "modeUp")
			modeUpKey = ConvertValue(value);
		
		else if (name == "modeDown")
			modeDownKey = ConvertValue(value);
	}

	void ReadConfig()
	{
		std::ifstream configFile("Config.txt", std::ios::in);
		std::string config;
		std::string tempLine;

		while (std::getline(configFile, tempLine))
			config += tempLine + "\n";
		
		configFile.close();
		
		enum class ReadMode { commentMode, nameMode, valueMode };
		ReadMode readMode = ReadMode::nameMode;
		std::string name;
		std::string value;
		
		for (int i = 0; i < config.size(); i++)
		{
			char c = config[i];
			
			if (c == ' ')
				continue;
			
			// comment mode
			if (c == '#')
			{
				if (readMode == ReadMode::valueMode)
				{
					SetValue(name, value);
					value = "";
					name = "";
				}
				readMode = ReadMode::commentMode;
				continue;
			}
			if (readMode == ReadMode::commentMode)
			{
				if (c == '\n')
					readMode = ReadMode::nameMode;
			}

			// name mode
			else if (readMode == ReadMode::nameMode)
			{
				if (c == '=')
					readMode = ReadMode::valueMode;
				else
					name.push_back(c);
			}

			// value mode
			else
			{
				if (c == '\n')
				{
					SetValue(name, value);
					value = "";
					name = "";
					readMode = ReadMode::nameMode;
				}
				else
					value.push_back(c);
			}

			continue;
		}
	}

	void RegisterHotKeys()
	{
		RegisterHotKey(NULL, 0, MOD_NOREPEAT, mouseMoveUpKey);
		RegisterHotKey(NULL, 0, MOD_NOREPEAT, mouseMoveDownKey);
		RegisterHotKey(NULL, 0, MOD_NOREPEAT, mouseMoveRightKey);
		RegisterHotKey(NULL, 0, MOD_NOREPEAT, mouseMoveLeftKey);
		RegisterHotKey(NULL, 0, MOD_NOREPEAT, mouseWheelUpKey);
		RegisterHotKey(NULL, 0, MOD_NOREPEAT, mouseWheelDownKey);
		RegisterHotKey(NULL, 0, MOD_NOREPEAT, mouseLeftKey);
		RegisterHotKey(NULL, 0, MOD_NOREPEAT, mouseRightKey);
		RegisterHotKey(NULL, 0, MOD_NOREPEAT, mouseMidKey);
		RegisterHotKey(NULL, 1, MOD_NOREPEAT | functionKey, exitKey);
		RegisterHotKey(NULL, 1, MOD_NOREPEAT | functionKey, activateKey);
		RegisterHotKey(NULL, 0, MOD_NOREPEAT | functionKey, modeUpKey);
		RegisterHotKey(NULL, 0, MOD_NOREPEAT | functionKey, modeDownKey);
	}	

	/*
	void PrintValues()
	{
		std::cout << mouseMoveUpKey << std::endl;
		std::cout << mouseMoveDownKey << std::endl;
		std::cout << mouseMoveRightKey << std::endl;
		std::cout << mouseMoveLeftKey << std::endl;
		std::cout << mouseWheelUpKey << std::endl;
		std::cout << mouseWheelDownKey << std::endl;
		std::cout << mouseLeftKey << std::endl;
		std::cout << mouseRightKey << std::endl;
		std::cout << mouseMidKey << std::endl;
		std::cout << functionKey << std::endl;
		std::cout << exitKey << std::endl;
		std::cout << activateKey << std::endl;
		std::cout << modeUpKey << std::endl;
		std::cout << modeDownKey << std::endl;
	}
	*/
}

using namespace Data;

void InputLoop()
{
	InitializeInputs();
	ReadConfig();
	RegisterHotKeys();

	while (true)
	{
		// exit
		if (GetAsyncKeyState(functionKey) && GetAsyncKeyState(exitKey))
		{
			if (exiting == false)
			{
				exitStart = std::chrono::high_resolution_clock::now();
				exiting = true;
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
			exiting = false;
		
		// activate
		if (GetAsyncKeyState(functionKey) && GetAsyncKeyState(activateKey))
		{
			if (!activating)
			{
				active = !active;
				activating = true;
				if (active)
					RegisterHotKeys();
				else
					UnregisterHotKey(NULL, 0);
			}
		}
		else
			activating = false;

		end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::this_thread::sleep_for(std::chrono::microseconds(mcrosecsPerTick - duration.count()));

		start = std::chrono::high_resolution_clock::now();

		if (!active)
			continue;
		
		modeUpButton = false;
		modeDownButton = false;
		movedY = false;
		movedX = false;
		movedW = false;
		
		// mode
		if (GetAsyncKeyState(functionKey) && GetAsyncKeyState(modeUpKey))
		{
			if (!modeUpButton)
			{
				modeUpButton = true;
				mode++;
				if (mode > 1)
					mode = 0;
			}
		}
		if (GetAsyncKeyState(functionKey) && GetAsyncKeyState(modeDownKey))
		{
			if (!modeDownButton)
			{
				modeDownButton = true;
				mode--;
				if (mode < 0)
					mode = 1;
			}
		}
		// down
		if (GetAsyncKeyState(mouseMoveDownKey))
		{
			movedY = true;
			if (ySpeed < maxSpeed)
				ySpeed += acceleration;
			if (ySpeed > maxSpeed)
				ySpeed = maxSpeed;
		}
		// up
		if (GetAsyncKeyState(mouseMoveUpKey))
		{
			movedY = true;
			if (ySpeed > -maxSpeed)
				ySpeed -= acceleration;
			if (ySpeed < -maxSpeed)
				ySpeed = -maxSpeed;
		}
		// right
		if (GetAsyncKeyState(mouseMoveRightKey))
		{
			movedX = true;
			if (xSpeed < maxSpeed)
				xSpeed += acceleration;
			if (xSpeed > maxSpeed)
				xSpeed = maxSpeed;
		}
		// left
		if (GetAsyncKeyState(mouseMoveLeftKey))
		{
			movedX = true;
			if (xSpeed > -maxSpeed)
				xSpeed -= acceleration;
			if (xSpeed < -maxSpeed)
				xSpeed = -maxSpeed;
		}
		// mouseL
		if (GetAsyncKeyState(mouseLeftKey))
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
		if (GetAsyncKeyState(mouseRightKey))
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
		if (GetAsyncKeyState(mouseMidKey))
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
		// wUp
		if (GetAsyncKeyState(mouseWheelUpKey))
		{
			if (wSpeed < wMaxSpeed)
				wSpeed += wAcceleration;
			if (wSpeed > wMaxSpeed)
				wSpeed = wMaxSpeed;
			movedW = true;
		}
		// wDown
		if (GetAsyncKeyState(mouseWheelDownKey))
		{
			if (wSpeed > -wMaxSpeed)
				wSpeed -= wAcceleration;
			if (wSpeed < -wMaxSpeed)
				wSpeed = -wMaxSpeed;
			movedW = true;
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
		mouseWheel.mi.mouseData = (unsigned long)(wSpeed);
		
		SendInput(1, &mouseMove, sizeofinput);
		SendInput(1, &mouseWheel, sizeofinput);
	}
}

// int main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	InputLoop();
}