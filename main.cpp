#define _WIN32_WINNT 0x0500 //for the no-resize window ;)
#include "funcs.h"
#include "clicker.h"

//mission: autoclicker that's not shit
//threads: use them?
//cpu: please no
//use _getch() to make autoclicker go up/down (in console window)
//use the getWindowName and up arrow to get window (this will require a thread??) [VK_UP]
//exit the program if the [`] or [~] key is pressed (0xC0)

static int g_clicks_per_second = 6;
std::string g_windowName{};
int g_click_key{};
int g_click_hold = 1;
bool g_left_active = true;
std::string g_active_key_print{};
bool g_human_hold_randomization = false;
int g_autoclicker_delay_divide = 999;
std::vector<std::string> static const loadingIcons{"|", "/", "--", "\\"}; //thx maddy lmao

namespace vanillaKeys
{
	constexpr int tilda_key = 96;
	constexpr int right_bracket = 93;
	constexpr int left_braket = 91;
	constexpr int lowercase_v = 118;
	constexpr int capital_v = 86;
	constexpr int f1_or_semicolon = 59;
}

namespace util
{
	void printCPS()
	{
		clicker::PrintAtCoords("CPS: " +
			std::to_string(g_clicks_per_second), 0, 3, true);
	}
	void printActiveWindow(bool useActiveWindow)
	{
		clicker::PrintAtCoords("Updating....", 0, 6, true);
		clicker::yield(200);
		if (useActiveWindow)
		{
			g_windowName = clicker::GetActiveWindowTitle();
		}
		clicker::PrintAtCoords(g_windowName, 0, 6, true);
	}
	void printActiveKey()
	{
		clicker::PrintAtCoords(g_active_key_print, 0, 10, true);
	}
	void printIfHumanRandomization()
	{
		clicker::PrintAtCoords("----Human Hold Randomization----", 0, 12, true);
		if (g_human_hold_randomization)
		{
			clicker::PrintAtCoords("Human randomization: TRUE", 0, 13, true);
		}
		else
		{
			clicker::PrintAtCoords("Human randomization: FALSE", 0, 13, true);
		}
	}
	void prePrint()
	{
		clicker::PrintAtCoords("----ScriptCat AutoClicker----", 0, 0, true);
		if (!g_left_active)
		{
			clicker::PrintAtCoords("----CPS-[RIGHT ACTIVE]----", 0, 2, true);
		}
		else
		{
			clicker::PrintAtCoords("----CPS-[LEFT ACTIVE]----", 0, 2, true);
		}
		util::printCPS();
		clicker::PrintAtCoords("----SELECTED WINDOW----", 0, 5, true);
		//selected window at line 6
		util::printActiveWindow(false);

		clicker::PrintAtCoords("----CLICK HOLD KEY (CAN'T BE LOWERCASE)----", 0, 9, true);
		//selected click key at line 10
		util::printActiveKey();
		util::printIfHumanRandomization(); //lines 12-13
	}
	void upDownCPS()
	{
		char a = _getch();
		Sleep(10);
		if (!a == 0) {
			const int vanilla = clicker::CharToKey(a);
			if (!(GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN) ||
				GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_LEFT)))
				//this statement is to stop the user from registering arrow keys as their keybinds for the autoclicker.
			{
				switch (vanilla)
				{
				case vanillaKeys::tilda_key:
					exit(707);
					break;
				case vanillaKeys::right_bracket:
					g_clicks_per_second++;
					util::printCPS();
					break;
				case vanillaKeys::left_braket:
					if (g_clicks_per_second > 1)
					{
						g_clicks_per_second--;
						util::printCPS();
					}
					break;
				case vanillaKeys::lowercase_v:
				case vanillaKeys::capital_v:
					g_left_active = !g_left_active;
					util::prePrint();
					break;
				case vanillaKeys::f1_or_semicolon:
					g_human_hold_randomization = !g_human_hold_randomization;
					util::prePrint();
					break;
				default:
					g_click_key = vanilla;
					std::string to_print = std::format("Click key: {} {} {}", a, " || ", vanilla);
					clicker::PrintAtCoords(to_print, 0, 10, true);
					g_active_key_print = to_print;
					break;
				}
			}
		}
	}
	void exitDetection()
	{
		while (1)
		{
			if (GetAsyncKeyState(0xC0)) //0xC0 is tilda key
			{
				clicker::fillExit();
				exit(712);
			}
			clicker::yield(400);
		}
	}
	void upArrowWindow()
	{
		while (1)
		{
			clicker::yield(100);
			if (GetAsyncKeyState(VK_UP))
			{
				util::printActiveWindow(true);
			}
			else if (GetAsyncKeyState(VK_DOWN))
			{
				g_windowName = "NONE";
				util::printActiveWindow(false);
			}

		}
	}
	void clickKeyDetection()
	{
		while (1)
		{
			if (g_human_hold_randomization)
			{
				g_autoclicker_delay_divide = 600;
			}
			else
			{
				//trying to compensate here.
				//this is really bad. Don't fucking do this.
				//I'm doing this b/c im braindead.
				g_autoclicker_delay_divide = g_clicks_per_second < 25 ? 999 : 950;
			}
			if (GetAsyncKeyState(g_click_key) && (clicker::GetActiveWindowTitle() == g_windowName || g_windowName == "NONE"))
			{
				if (g_human_hold_randomization)
				{
					g_click_hold = clicker::randomNum(25, 40); //randomization of click holding, simulating human.
				}
				if (g_left_active)
				{
					clicker::leftClickMouse(g_click_hold);
				}
				else
				{
					clicker::rightClickMouse(g_click_hold);
				}
			}
			clicker::yield(g_autoclicker_delay_divide / g_clicks_per_second); //600 instead of 1000 to compensate
		}
	}
	void funnyLoadingConsole()
	{
		while (1)
		{
			for (int i = 0; i < loadingIcons.size(); i++)
			{
				std::string toSet = "ScriptCat AutoClicker    " + loadingIcons[i];
				SetConsoleTitleA(toSet.c_str());
				clicker::yield(500);
			}
		}
	}
}

int main()
{
	//this is to prevent the user from resizing the console.
	//taken from https://stackoverflow.com/questions/47358043/can-i-prevent-the-user-of-my-program-to-resize-the-console-window-in-c
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);


	util::prePrint(); //pre-print the text

	//thread for [`] detection (EXIT)
	std::thread exit_detection(util::exitDetection);
	exit_detection.detach();

	//thread for detecting up arrow key
	std::thread up_arrow(util::upArrowWindow);
	up_arrow.detach();

	//thread for detecting the click key.
	std::thread click_detection(util::clickKeyDetection);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	click_detection.detach();

	//thread for setting loading console title
	std::thread loading_title(util::funnyLoadingConsole);
	loading_title.detach();

	while (1)
	{
		//this is for the `[` and `]` detection, up/down on the CPS.
		//also for 'v' and 'V' detection, for right and left-clicking.
		util::upDownCPS();
		Sleep(1);
	}
}