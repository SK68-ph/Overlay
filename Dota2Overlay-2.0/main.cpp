#include "hMain.h"
#include "Dota2MemReader.h"

HWND hWnd, TargetWnd;
MSG Message;
RECT WindowRect, ClientRect;
int windowWidth, windowHeight, overlaySize;
int clientWidth , clientHeight;
int borderWidth, borderHeight; // Grenz Größen

char lWindowName[256] = "GG";
char tWindowName[256] = "Dota 2"; // put Game window name here

const MARGINS pMargin = { 0,0, clientWidth, clientHeight };


LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{

	case WM_PAINT: // we need to paint? lets paint!
		Render();
		break;
	case WM_CREATE:
		return DwmExtendFrameIntoClientArea(hWnd, &pMargin); // extension of window frame into client area
		break;
	case WM_DESTROY:
		PostQuitMessage(0); // We need to use this to exit a message loop
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam); // Making sure all messages are processed
		break;
	}
}

void gen_random(char* s, const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len] = 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	gen_random(lWindowName, 20);
	WNDCLASSEXA OverlayWnd; // contains window class information
	OverlayWnd.cbSize = sizeof(WNDCLASSEXA); // size of struct, basically checking for version or check
	OverlayWnd.style = CS_HREDRAW | CS_VREDRAW;  // Style, redraw method type
	OverlayWnd.lpfnWndProc = Proc; // Pointer to the window procedure
	OverlayWnd.cbClsExtra = 0; // window class struct extra bytes
	OverlayWnd.cbWndExtra = 0; // window instance extra bytes
	OverlayWnd.hInstance = hInstance; // handle to the instance that contains the window procedure for the class
	OverlayWnd.hIcon = LoadIcon(NULL, IDI_APPLICATION); // basic window icon set
	OverlayWnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // basic window icon set
	OverlayWnd.hCursor = LoadCursor(NULL, IDC_ARROW); // basic window cursor icon set
	OverlayWnd.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); // handle to the class background brush
	OverlayWnd.lpszMenuName = lWindowName;
	OverlayWnd.lpszClassName = lWindowName;

	// registers a window class for the use in call to this createwindowex func
	if (!RegisterClassExA(&OverlayWnd))
	{
		exit(1);
	}

	TargetWnd = FindWindowA(0, tWindowName);

	/*
	CreateWindowEx creates an overlapped, pop-up, or child window with an extended window style.

	dwExStyle (DWORD) is the extended window style of the window being created.
	WS_EX_TOPMOST means that the window should be placed above all non-topmost windows and should stay above them, even when the window is deactivated
	WS_EX_LAYERED uses a layered window can significantly improve performance and visual effects for a window that has a complex shape, animates its shape, or wishes to use alpha blending effects.
	WS_EX_COMPOSITED paints all descendants of a window in bottom-to-top painting order using double-buffering.
	WS_EX_TRANSPARENT means that the window should not be painted until siblings beneath the window (that were created by the same thread) have been painted.

	lpClassName (LPCTSTR) is a null-terminated string or a class atom created by a previous call to the RegisterClass or RegisterClassEx function.
	lpWindowName (LPCSTR) is the window name.
	dwStyle (DWORD) is the style of the window being created.
	WS_POPUP means that the window is a pop-up window.
	x (int) is the horizontal position of the window.
	y (int) is the vertical position of the window.
	nWidth (int) is the width.
	nHeight (int) is the height.

	The last three nulls are all optional, and I wont bother mentioning them. If you are interested google CreateWindowEx.

	The dimensions for the overlay will be resized when the game is found.
	*/


	if (TargetWnd)
	{
		// Adjust Overlay text size and location based on resolution
		overlaySize = 45;
		GetWindowRect(TargetWnd, &WindowRect);
		windowWidth = WindowRect.right - WindowRect.left;
		windowHeight = WindowRect.bottom - WindowRect.top;
		clientHeight = windowHeight;
		clientWidth = windowWidth;

		_RPT1(0, "WindowWidth = %u\n", windowWidth);
		overlaySize = 45;
		hWnd = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, lWindowName, lWindowName, WS_POPUP, 1, 1, windowWidth, windowHeight, 0, 0, 0, 0);
	}


	/*
	SetLayeredWindowAttributes sets the opacity and transparency color key for a layered window.
	*/
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
	SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);

	/*
	Show the layered window aka our overlay.
	*/
	ShowWindow(hWnd, SW_SHOW);
	if (initHack() == -1)
	{
		MessageBox(NULL, L"Error", L"Error", 0);
		
	}
	/*
	We use our handle to our overlay and initalize our D3D adapter.
	*/
	D3D9Init(hWnd);

	/*
	While Delete key is not pressed, we will be enable our hack.
	*/
	while (!GetAsyncKeyState(VK_DELETE))
	{

		/*
		Dispatches incoming sent messages, checks the thread message queue for a posted message, and retrieves the message (if any exist). Messages are removed from the queue after processing due to PM_REMOVE.
		*/
		if (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			/*
			Translates virtual-key messages into character messages.
			*/
			TranslateMessage(&Message);

			/*
			Dispatches a message to a window procedure.
			*/
			DispatchMessage(&Message);
		}

		/*
		get the hWnd for the game we want.
		*/
		TargetWnd = FindWindowA(0, tWindowName);

		/*
		If there is no game, quit.
		*/
		if (!TargetWnd)
			exit(0);

		/*
		Set the RECT using the targeted window.
		*/
		GetWindowRect(TargetWnd, &WindowRect);
		GetClientRect(TargetWnd, &ClientRect);

		windowWidth = WindowRect.right - WindowRect.left;
		windowHeight = WindowRect.bottom - WindowRect.top;

		/*
		Resize the overlay matching what is required.
		*/
		DWORD dwStyle = GetWindowLong(TargetWnd, GWL_STYLE);

		if (dwStyle & WS_BORDER)
		{
			windowHeight = WindowRect.bottom - WindowRect.top;
			windowWidth = WindowRect.right - WindowRect.left;
			clientHeight = ClientRect.bottom - ClientRect.top;
			clientWidth = ClientRect.right - ClientRect.left;
			borderHeight = (windowHeight - ClientRect.bottom);
			borderWidth = (windowWidth - ClientRect.right) / 2; //only want one side
			borderHeight -= borderWidth; //remove bottom from width, bottom is the same size as either side so we subtract that

			WindowRect.left += borderWidth;
			WindowRect.top += borderHeight;
		}

		MoveWindow(hWnd, WindowRect.left, WindowRect.top, clientWidth, clientHeight, true);

	} // End of Panic Loop

	/*
	Lets exit immediately...
	*/
	close();
	exit(0);

}