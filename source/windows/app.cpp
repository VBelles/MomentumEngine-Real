#include "mcv_platform.h"
#include "app.h"
#include "render/render.h"
#include "input/devices/mouse.h"
#include "profiling/profiling.h"
#include <windowsx.h>

CApp* CApp::app_instance = nullptr;

//--------------------------------------------------------------------------------------
LRESULT CALLBACK CApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	// If the OS processes it, do not process anymore
	if (CEngine::get().getModules().OnOSMsg(hWnd, message, wParam, lParam))
		return 1;

	switch (message) {

	case CDirectoyWatcher::WM_FILE_CHANGED:
	{
		const char* filename = (const char*)lParam;
		dbg("File has changed! %s (%d)\n", filename, wParam);
		Resources.onFileChanged(filename);
		delete[] filename;
		break;
	}

	case WM_PAINT:
		// Validate screen repaint in os/windows 
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse) {
			int posX = GET_X_LPARAM(lParam);
			int posY = GET_Y_LPARAM(lParam);
			mouse->setPosition(static_cast<float>(posX), static_cast<float>(posY));
			app_instance->resetMouse = true;
		}
	}
	break;

	case WM_MBUTTONDOWN:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse) {
			mouse->setButton(Input::MOUSE_MIDDLE, true);
			SetCapture(hWnd);
		}
	}
	break;

	case WM_MBUTTONUP:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse) {
			mouse->setButton(Input::MOUSE_MIDDLE, false);
			ReleaseCapture();
		}
	}
	break;

	case WM_LBUTTONDOWN:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse) {
			mouse->setButton(Input::MOUSE_LEFT, true);
			SetCapture(hWnd);
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse) {
			mouse->setButton(Input::MOUSE_LEFT, false);
			ReleaseCapture();
		}
	}
	break;

	case WM_RBUTTONDOWN:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse) {
			mouse->setButton(Input::MOUSE_RIGHT, true);
			SetCapture(hWnd);
		}
	}
	break;

	case WM_RBUTTONUP:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse) {
			mouse->setButton(Input::MOUSE_RIGHT, false);
			ReleaseCapture();
		}
	}
	break;

	case WM_MOUSEWHEEL:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse) {
			float wheel_delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
			mouse->setWheelDelta(wheel_delta);
		}
	}
	break;
	case WM_INPUT:
	{
		UINT dwSize = 64;
		static BYTE lpb[64];

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
			lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE) {
			int xPosRelative = raw->data.mouse.lLastX;
			int yPosRelative = raw->data.mouse.lLastY;
			Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
			if (mouse) {
				mouse->setDelta(static_cast<float>(xPosRelative), static_cast<float>(yPosRelative));
			}
		}
		break;
	}
	case WM_SETFOCUS:
		if(Engine.isStarted())
			EngineInput.setActive(true);
		app_instance->isWindowFocused = true;
		break;

	case WM_KILLFOCUS:
		if (Engine.isStarted()) {
			EngineInput.setActive(false);
		}
		app_instance->isWindowFocused = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
bool CApp::createWindow(HINSTANCE new_hInstance, int nCmdShow) {
	hInstance = new_hInstance;

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "MCVWindowsClass";
	wcex.hIconSm = NULL;
	if (!RegisterClassEx(&wcex)) return false;

	// Create window
	if (!fullscreen) {
		RECT rc = { 0, 0, static_cast<LONG>(resolution.x), static_cast<LONG>(resolution.y) };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		hWnd = CreateWindow("MCVWindowsClass", "Momentum",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
			NULL);
	}
	else {
		hWnd = CreateWindow("MCVWindowsClass", "Momentum",
			WS_POPUP,
			0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			NULL, NULL, hInstance, NULL);
	}

	if (!hWnd) return false;

	ShowWindow(hWnd, nCmdShow);

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = hWnd;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

	return true;
}

//--------------------------------------------------------------------------------------
// Process windows msgs, or if nothing to do, generate a new frame
//--------------------------------------------------------------------------------------
void CApp::mainLoop() {
	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message) {
		// Check if windows has some msg for us
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			doFrame();
		}
	}
}

//--------------------------------------------------------------------------------------
// Read any basic configuration required to boot, initial resolution, full screen, modules, ...
//--------------------------------------------------------------------------------------
bool CApp::readConfig() {
	json j = loadJson("data/configuration.json");
	json& jScreen = j["screen"];

	resolution = loadVEC2(jScreen["resolution"]);
	fullscreen = jScreen["fullscreen"];
	showDebug = j["showDebug"];
	ShowCursor(showDebug);
	//16:9 resolutions:
	//1280x720  = 720p HD
	//1920x1080 = 1080p HD
	//2560x1440 = 27' Monitor
	//3840x2160 = 4K UHDTV
	//5120x2880 = Retina 5K
	//7680x4320 = 8K UHDTV

	time_since_last_render.reset();

	Engine.getRender().configure(static_cast<int>(resolution.x), static_cast<int>(resolution.y));
	return true;
}

//--------------------------------------------------------------------------------------
bool CApp::start() {
	resources_dir_watcher.start("data", getWnd());

	return Engine.start();
}

//--------------------------------------------------------------------------------------
bool CApp::stop() {
	return Engine.stop();
}

//--------------------------------------------------------------------------------------
void CApp::doFrame() {
	PROFILE_FRAME_BEGINS();
	PROFILE_FUNCTION("App::doFrame");

	/*double frameTime = time_since_last_render.elapsedAndReset();
	if (frameTime > 0.25)
		frameTime = 0.25;

	accumulator += frameTime;

	while (accumulator >= dt) {
		Engine.update(dt);
		accumulator -= dt;

		if (EngineInput["debug_mode"].getsPressed()) {
			showDebug = !showDebug;
			ShowCursor(showDebug);
		}

		if (!showDebug && isWindowFocused) {
			RECT rect;
			GetWindowRect(getWnd(), &rect);
			SetCursorPos((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2);
		}
	}

	Engine.render();*/

	double frameTime = time_since_last_render.elapsedAndReset();
	Engine.update(frameTime);
	Engine.render();
}
