#pragma once
#include "funcs.h"

namespace clicker
{
	extern const std::string dead_spaces = std::string(200, ' ');
	void startup()
	{
		SetConsoleTitleA("ScriptCat Clicker || TILDA TO EXIT");
		std::cout << "Remember: press UP ARROW to select current active window!" << '\n';
	}
	std::string GetActiveWindowTitle()
	{
		//tysm quicknet!
		HWND handle = GetForegroundWindow();
		char buf[500]{};
		GetWindowTextA(handle, buf, 500);
		std::string const window_title = buf;
		return window_title;
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
	void PrintAtCoords(int text, SHORT x, SHORT y, bool clearLine)
	{
		COORD c = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		if (clearLine) { std::cout << clicker::dead_spaces; }
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		std::cout << std::to_string(text);
	}
	int randomNum(int bottom, int top)
	{
		return (rand() % top + bottom);
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
			clicker::PrintAtCoords("Exiting...", i, i, true);
			clicker::yield(10);
		}
	}
}