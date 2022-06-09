#pragma once
#include "funcs.h"

namespace clicker
{
	extern const std::string dead_spaces = std::string(200, ' ');
	std::string afterLastBackslash(std::string input)
	{
		size_t const pos = input.find_last_of('\\');
		return input.substr(pos + 1);
	}
	std::string GetActiveWindowTitle()
	{
		const HWND topHandle = GetForegroundWindow(); //window handle of foreground window
		DWORD pid{}; //pid of process (later usage)
		GetWindowThreadProcessId(topHandle, &pid); //sets the pid to the pid of foreground window
		HANDLE processHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, pid); //getting handle of the process
		char buf[500]{};
		GetProcessImageFileNameA(processHandle, buf, 500); //puts the path to the .exe into [buf]
		std::string bufStr = buf; //make it into a string
		return (clicker::afterLastBackslash(bufStr)); //return the string after last backslash, which is .exe name.
	}
	
	void leftClickMouse(int sleepTime)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		Sleep(sleepTime);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
	void rightClickMouse(int sleepTime)
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
		Sleep(sleepTime);
		mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	}
	SHORT GetVKFromChar(int ch)
	{
		return VkKeyScanA(ch);
	}
	int CharToKey(char buf)
	{
		int ret;
		ret = static_cast<int>(buf);
		return ret;
	}
	void PrintAtCoords(std::string text, SHORT x, SHORT y, bool clearLine)
	{
		COORD c = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		if (clearLine) { std::cout << clicker::dead_spaces; }
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		std::cout << text;
	}
	inline void PrintAtCoords(int text, SHORT x, SHORT y, bool clearLine)
	{
		clicker::PrintAtCoords(std::to_string(text), x, y, clearLine);
	}
	int randomNum(int bottom, int top)
	{
		srand(unsigned(time(0)));
		return (rand() % top + bottom); //femboy programming
	}
	std::string get_printActiveWindow()
	{
		std::string active_window = clicker::GetActiveWindowTitle();
		std::string toprint = "Selected window: " + active_window;
		clicker::PrintAtCoords(toprint, 0, 5, 1);
		return active_window;
	}
	inline void yield(int time)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(time));
	}
	void fillExit()
	{
		for (int i = 0; i < 50; i++)
		{
			SetConsoleTitleA("Exiting.....");
			clicker::PrintAtCoords("Exiting...", i, i, true);
			clicker::yield(10);
		}
	}
	bool isKeyJustPressed(int key)
	{
		if (GetAsyncKeyState(key))
		{
			clicker::yield(100);
			if (!GetAsyncKeyState(key))
			{
				return true;
			}
		}
		return false;
	}
}