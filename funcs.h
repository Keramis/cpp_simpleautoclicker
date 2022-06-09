#pragma once
#pragma comment(lib, "psapi.lib") //tysm quicknet, linker wasn't looking for psapi.lib, told me this workaround
#include <iostream> //gotta have this as well
#include <string> //you gotta have strings
#include <vector> //simple arrays
#include <algorithm>
#include <conio.h> //for immediate right/left bracket.
#include <cmath>
#include <concrt.h> //for sleep function (ty kreeako)
#include <Windows.h> //key detection
#include <WinUser.h> //virtual keycodes
#include <Psapi.h>	//get window name, process api
#include <oleacc.h>
#include <thread>	//threads to actually do smth
#include <chrono>
#include <time.h>