#include "funcs.h"
#include "clicker.h"

using std::cout;
using std::string;
constexpr auto tilda_key = 0xC0;
std::string active_window{};

static int g_cps = 10;

namespace clickthreads
{
	using namespace std::chrono_literals;
	void right_arrow() {
		while (1)
		{
			if (clicker::isKeyJustPressed(VK_RIGHT))
			{
				++g_cps;
				clicker::PrintAtCoords("CPS: " + std::to_string(g_cps), 0, 8, true);
			}
		}
	}
	void left_arrow()
	{
		while (1)
		{
			if (clicker::isKeyJustPressed(VK_LEFT))
			{
				--g_cps;
				clicker::PrintAtCoords("CPS: " + std::to_string(g_cps), 0, 8, true);
			}
		}
	}
	void up_arrow()
	{
		while (1) {
			if (clicker::isKeyJustPressed(VK_UP))
			{
				active_window = clicker::get_printActiveWindow();
			}
		}
	}
	void click(int clickKey)
	{
		while (1)
		{
			if (clicker::GetActiveWindowTitle() == active_window)
			{
				clicker::leftClickWithKey(clickKey);
				Sleep(980 / g_cps); //dividing by 980 to compensate for exe slowness, this works best.
			}
		}
	}
	void endEverything()
	{
		while (1)
		{
			if (clicker::isKeyJustPressed(tilda_key))
			{
				exit(1);
			}
		}
	}
}

//thank you Sai for telling me to do thread.detach!

int main()
{

	clicker::startup();
	int clickKey = clicker::askForClickerKey();

	//starting threads

	//right arrow detection (more CPS)
	std::thread right_detection(clickthreads::right_arrow);
	right_detection.detach();

	//left arrow detection (less CPS)
	std::thread left_detection(clickthreads::left_arrow);
	left_detection.detach();

	//up arrow detection (window selection)
	std::thread up_detection(clickthreads::up_arrow);
	up_detection.detach();

	//actual click thread
	std::thread click_mouse(clickthreads::click, clickKey);
	click_mouse.detach();

	//tilda key detection thread
	std::thread tilda_detection(clickthreads::endEverything);
	tilda_detection.detach();

	//pre-printing here:
	clicker::PrintAtCoords("-----ACTIVE WINDOW-----", 0, 4, false);
	clicker::PrintAtCoords("-----CPS-----", 0, 7, false);
	clicker::PrintAtCoords("CPS: " + std::to_string(g_cps), 0, 8, false);

	//this keeps the program from just returning 404.
	//the actual tilda key detection is in a thread.
	//probably a dogshit way of doing so but idc
	while (!clicker::isKeyJustPressed(tilda_key))
	{
		Sleep(1);
	}

	//return 404 if we somehow got here. this is very wrong if we did so.
	return 404;
}
