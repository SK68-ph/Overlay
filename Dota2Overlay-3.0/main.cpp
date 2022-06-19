#include "DirectOverlay.h"
#include "include.h"
#include "Dota2MemReader.h"

int windowWidth, windowHeight, overlayScale, overlayX_Pos, overlayY_Pos;
float r = 1;
float g = 0;
float b = 0;
float a = 1;
int fontSize = 50;
bool init = false;
HWND tWindow;
char tWindowName[256] = "Dota 2";
char hWindowName[256] = "Dota 2";


// Restore previous position and fontsize from Config
void getConfig() {
	std::string line;
	std::ifstream config("overlay.ini");
	if (config.is_open()) {
		if (config.peek())
		{
			std::cout << "Config loading ..." << std::endl;
			std::getline(config, line); if (true)
				overlayX_Pos = std::stoi(line);
			std::getline(config, line);
			overlayY_Pos = std::stoi(line);
			std::getline(config, line);
			fontSize = std::stoi(line);
		}
	}
	config.close();

}

void setConfig() {
	std::fstream config;
	config.open("overlay.ini", std::ios::out | std::ios::out);
	if (config.is_open())
	{
		config << overlayX_Pos << std::endl;
		config << overlayY_Pos << std::endl;
		config << fontSize << std::endl;

	}
	config.close();
}

int pVbe = 0; // Previous vbe value

void drawLoop(int width, int height) {

	// if insert is pressed we move our TextOverlay to current cursor position
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		POINT p;
		if (GetCursorPos(&p))
		{
			overlayX_Pos = p.x;
			overlayY_Pos = p.y;
			setConfig();
		}

	}
	// if PageUp is pressed we increase font size
	if (GetAsyncKeyState(VK_PRIOR) & 1)
	{
		fontSize += 2;
		setConfig();
	}
	// if PageDown is pressed we decrease font size
	else if (GetAsyncKeyState(VK_NEXT) & 1)
	{
		fontSize -= 2;
		setConfig();
	}

	if (tWindow == GetForegroundWindow())
	{
		int vbe = getVbe();
		if (vbe == 0 && pVbe == 0 )
		{
			DrawString("Visible", fontSize, overlayX_Pos, overlayY_Pos, r, g, b, a);
		}
		pVbe = vbe;
	}
	Sleep(100);
}

void main()
{
	// Window
	std::cout << "Waiting for dota2.exe:\t";
	tWindow = FindWindowA(0, tWindowName);
	if (tWindow == 0)
	{
		std::cout << "Waiting..";
		while (true)
		{
			Sleep(1000);
			tWindow = FindWindowA(0, tWindowName);
			if (tWindow != 0) { break; }
		}
	}
	std::cout << "Game found ..." << std::endl;

	std::cout << "Initializing ..." << std::endl;
	initHack(); 
	getConfig();
	DirectOverlaySetOption(D2DOV_FONT_IMPACT | D2DOV_REQUIRE_FOREGROUND);
	DirectOverlaySetup(drawLoop, tWindow);
	std::cout << "Done ..." << std::endl ;
	std::cout << "\nHotkeys :"<< std::endl;
	std::cout << "INSERT = Move Overlay to current position of cursor" << std::endl;
	std::cout << "PAGE_UP = Increase Font Size" << std::endl;
	std::cout << "PAGE_DOWN = Decrease Font Size" << std::endl;
	std::cout << "\nPress enter to exit... \t" << std::endl;
	system("pause");
}
