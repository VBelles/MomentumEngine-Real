#include "mcv_platform.h"
#include "app.h"
#include "render/render.h"
#include "input/devices/mouse.h"
#include "profiling/profiling.h"
#include <windowsx.h>

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

CApp* CApp::app_instance = nullptr;
DWORD defaultStickyKeysState;

LRESULT CALLBACK CApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	// If the OS processes it, do not process anymore
	if (EngineModules.OnOSMsg(hWnd, message, wParam, lParam))
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
		if (Engine.isStarted())
			EngineInput.setActive(true);
		app_instance->setWindowFocused(true);

		STICKYKEYS sk;
		sk.cbSize = sizeof(sk);
		//Get user default sticky keys state
		SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(sk), &sk, 0);
		defaultStickyKeysState = sk.dwFlags;
		//Disable sticky keys
		sk.cbSize = sizeof(sk);
		sk.dwFlags = SKF_AVAILABLE;
		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(sk), &sk, 0);
		break;

	case WM_KILLFOCUS:
		if (Engine.isStarted()) {
			EngineInput.setActive(false);
		}
		app_instance->setWindowFocused(false);

		//Set user default sticky keys state
		sk.cbSize = sizeof(sk);
		sk.dwFlags = defaultStickyKeysState;
		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(sk), &sk, 0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void CApp::getDesktopResolution(int& horizontal, int& vertical) {
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
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
		RECT rc = { 0, 0, resX, resY };
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

	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = hWnd;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

	showCursor = true;

	return true;
}

//--------------------------------------------------------------------------------------
// Process windows msgs, or if nothing to do, generate a new frame
//--------------------------------------------------------------------------------------
void CApp::mainLoop() {
	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message && !stopMainLoop) {
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
	json& jCamera = j["camera"];
	json& jSound = j["sound"];

	std::string res = jScreen.value("resolution", "auto");
	if (res == "auto") {
		getDesktopResolution(resX, resY);
	}
	else {
		int n = sscanf(res.c_str(), "%d %d", &resX, &resY);
		if (n != 2) getDesktopResolution(resX, resY);
	}
	Engine.globalConfig.resolution = VEC2(resX, resY);
	fullscreen = jScreen.value("fullscreen", true);
	Engine.globalConfig.fullscreen = fullscreen;
	bool vsync = jScreen.value("vsync", false);
	Engine.globalConfig.vSync = vsync;
	//16:9 resolutions:
	//1280x720  = 720p HD
	//1920x1080 = 1080p HD
	//2560x1440 = 27' Monitor
	//3840x2160 = 4K UHDTV
	//5120x2880 = Retina 5K
	//7680x4320 = 8K UHDTV

	Engine.globalConfig.cameraAxis.x = jCamera.value("invert_x_axis", false) ? -1.f : 1.f;
	Engine.globalConfig.cameraAxis.y = jCamera.value("invert_y_axis", false) ? -1.f : 1.f;

	Engine.globalConfig.cameraSpeed = VEC2(jCamera.value("camera_speed", 0.5f), jCamera.value("camera_speed", 0.5f));
	Engine.globalConfig.cameraSpeedPad = VEC2(jCamera.value("camera_speed_pad", 2.0f), jCamera.value("camera_speed_pad", 2.0f));

	if (j.count("sound")) {
		Engine.globalConfig.masterVolume = jSound.value("master_volume", 1.f);
		Engine.globalConfig.musicVolume = jSound.value("music_volume", 1.f);
		Engine.globalConfig.soundVolume = jSound.value("sound_volume", 1.f);
	}

	time_since_last_render.reset();

	EngineRender.configure(resX, resY, vsync);


	return true;
}

bool CApp::start() {
	resources_dir_watcher.start("data", getWnd());

	return Engine.start();
}

bool CApp::stop() {
	return Engine.stop();
}

void CApp::setDebugMode(bool debug) {
	this->debug = debug;
	setResetMouse(!debug);
}

void CApp::setWindowFocused(bool windowFocused) {
	this->windowFocused = windowFocused;
}

bool CApp::shoulResetMouse() {
	return resetMouse && isWindowFocused();
}

void CApp::setResetMouse(bool resetMouse) {
	this->resetMouse = resetMouse;

	if (resetMouse && showCursor) {
		showCursor = false;
		int res = ShowCursor(showCursor);
	}
	else if (!resetMouse && !showCursor) {
		showCursor = true;
		int res = ShowCursor(showCursor);
	}
}


void CApp::doFrame() {
	PROFILE_FRAME_BEGINS();
	PROFILE_FUNCTION("App::doFrame");
	//ShowCursor(showCursor);
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

	float frameTime = time_since_last_render.elapsedAndReset();
	Engine.update(frameTime);
	Engine.render();
}
