//#include "includes.h"
#include "funcs.h"
//for loop: (int i = 0; i < limit; i++)
///vector for loop: for (int x : vector)

//compare strings using either [==] or [.compare]
/// [s] and [t] are strings. if you compare using ==,
/// it will return true or false; true if they are the same.
/// if you compare using: s.compare(t), it will return -1 if 
/// [s] is "lower than" [t], +1 if [s] is "greater than" [t],
/// and 0 if [s] is equal to [t] 

//if statements can be written arithmetically as: (condition) ? expression1 : expression2
/// this is the same thing as doing if (condition) {expression1} else {expression2}
/// we can actually use this to set values; value = (condition) ? expression1 : expression2
/// this will set value to expression1 if condition is true, else set it to expression2.

//using the [extern] keyword, we can make [const] variables have external linkage.
/// they are internal by default.
/// likewise, using the [static] keyword will make other variables internal, and have no linkage
/// [static] makes variables unable to be reset. This means that this variable can only be
/// initialized once, unlike "local" variables in functions.

namespace oof
{
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
	SHORT GetVKFromChar(int ch)
	{
		return VkKeyScanA(ch);
	}
	int CinCharToKey()
	{
		char buf;
		int ret;
		std::cin >> buf;
		ret = static_cast<int>(buf);
		return ret;
	}
	void PrintAtCoords(std::string text, int x, int y)
	{
		COORD c = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		std::cout << text;
	}
	int randomNum(int bottom, int top)
	{
		return (rand() % top + bottom);
	}
}

std::string deadSpaces = "                                                                                                                                                          ";

int main()
{
	using std::cout;
	using std::string;
	SetConsoleTitleA("Scriptcat autoclicker || ALPHA || TILDA TO QUIT");
	std::cout << "Please enter a key, IN CAPS, that you want to HOLD for the autoclicker!" << '\n'; //coord 0
	int keytocheck = oof::CinCharToKey(); //coord 1
	auto consoleHandle = GetActiveWindow();
	//
	string window_title;
	int click = 0;
	bool autoclicker_on = false;
	//setup done
askForCPS:
	oof::PrintAtCoords("Bottom limit CPS: ", 0, 2);
	int bottomCPS{};
	std::cin >> bottomCPS;
	oof::PrintAtCoords(deadSpaces, 0, 4);
	oof::PrintAtCoords("Top limit CPS: ", 0, 3);
	int topCPS{};
	std::cin >> topCPS;
	if (topCPS < bottomCPS)
	{
		oof::PrintAtCoords("Top limit cannot be less than bottom limit!", 0, 4);
		return -1;
	}
	oof::PrintAtCoords("Press UP ARROW to determine your window!", 0, 5);
	//0xC0 is ` or ~, the tilda.
	while (!GetAsyncKeyState(0xC0))
	{
		if (oof::isKeyJustPressed(VK_UP))
		{
			window_title = oof::GetActiveWindowTitle();
			oof::PrintAtCoords("----SELECTED WINDOW----", 0, 7);
			std::string str = "You have chosen " + window_title;
			oof::PrintAtCoords(deadSpaces, 0, 8);
			oof::PrintAtCoords(str, 0, 8);
		}
		if (oof::GetActiveWindowTitle() == window_title)
		{
			if (GetAsyncKeyState(keytocheck))
			{
				oof::leftClickMouse(1);
				Sleep(1000 / (oof::randomNum(bottomCPS, topCPS)));
			}
		}
	}
	
	return 0;
}