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
	bool isKeyJustReleased(DWORD virtualKey)
	{
		if (GetAsyncKeyState(virtualKey))
		{
			Sleep(1);
			if (!GetAsyncKeyState(virtualKey))
			{
				return true;
			}
		}
		return false;
	}
	bool isKeyJustPressed(DWORD virtualKey)
	{
		if (!GetAsyncKeyState(virtualKey))
		{
			Sleep(1);
			if (GetAsyncKeyState(virtualKey))
			{
				return true;
			}
		}
		return false;
	}
	std::string GetActiveWindowTitle()
	{
		//tysm quicknet!
		HWND handle = GetForegroundWindow();
		char buf[1000]{};
		GetWindowTextA(handle, buf, 1000);
		std::string const window_title = buf;
		return window_title;
	}
	void leftClickMouse(int sleepTime)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		Sleep(sleepTime);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
	void leftClickWithKey(int key)
	{
		if (GetAsyncKeyState(key))
		{
			leftClickMouse(1);
		}
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
	int randomNum(int bottom, int top)
	{
		return (rand() % top + bottom);
	}
	int askForClickerKey()
	{
		std::cout << "Please enter the key you want, IN ALL CAPS, for the clicker!" << '\n';
		char click_key{};
		std::cin >> click_key;
		return (CharToKey(click_key));
	}
	std::string get_printActiveWindow()
	{
		std::string active_window = clicker::GetActiveWindowTitle();
		std::string toprint = "Selected window: " + active_window;
		clicker::PrintAtCoords(toprint, 0, 5, 1);
		return active_window;
	}
}