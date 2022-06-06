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

namespace vanillaKeys
{
	extern constexpr int tilda_key = 96;
	extern constexpr int right_bracket = 93;
	extern constexpr int left_braket = 91;
	extern constexpr int lowercase_v = 118;
	extern constexpr int capital_v = 86;
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
	}
	void upDownCPS()
	{
		char a = _getch();
		Sleep(10);
		if (!a == 0) {
			const int vanilla = clicker::CharToKey(a);
			switch (vanilla)
			{
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
			default:
				g_click_key = vanilla;
				std::string to_print = std::format("Click key: {} {} {}", a, " || ", vanilla);
				clicker::PrintAtCoords(to_print, 0, 10, true);
				g_active_key_print = to_print;
				break;
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
			if (GetAsyncKeyState(g_click_key) && (clicker::GetActiveWindowTitle() == g_windowName || g_windowName == "NONE"))
			{
				if (g_left_active)
				{
					clicker::leftClickMouse(g_click_hold);
				}
				else
				{
					clicker::rightClickMouse(g_click_hold);
				}
			}
			clicker::yield(999 / g_clicks_per_second); //800 instead of 1000 to compensate
		}
	}
}

int main()
{
	util::prePrint(); //pre-print the text

	//thread for [`] detection (EXIT)
	std::thread exit_detection(util::exitDetection);
	exit_detection.detach();

	//thread for detecting up arrow key
	std::thread up_arrow(util::upArrowWindow);
	up_arrow.detach();

	//thread for detecting the click key.
	std::thread click_detection(util::clickKeyDetection);
	click_detection.detach();

	while (1)
	{
		//this is for the `[` and `]` detection, up/down on the CPS.
		//also for 'v' and 'V' detection, for right and left-clicking.
		util::upDownCPS();
		Sleep(1);
	}
}