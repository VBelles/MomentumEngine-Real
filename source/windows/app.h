#pragma once

#include "utils/directory_watcher.h"

class CApp {
private:
	// Windows stuff
	HWND hWnd;
	HINSTANCE hInstance;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	CTimer           time_since_last_render;
	CDirectoyWatcher resources_dir_watcher;

	static CApp* app_instance;

	bool debug = false;
	bool resetMouse = false;
	bool windowFocused = false;
	bool showCursor = false;

	//double dt = 1.f / 60.f;
	//double accumulator = 0.0;

public:
	VEC2 resolution;
	bool fullscreen = false;

	bool stopMainLoop = false;

	static CApp& get() {
		assert(app_instance);
		return *app_instance;
	}

	HWND getWnd() const { return hWnd; }

	CApp() {
		assert(app_instance == nullptr);
		app_instance = this;
	}

	~CApp() {
		assert(app_instance == this);
		app_instance = nullptr;
	}

	// Remove copy ctor
	CApp(const CApp&) = delete;

	bool readConfig();
	bool createWindow(HINSTANCE new_hInstance, int nCmdShow);
	bool start();
	void mainLoop();
	void doFrame();
	bool stop();

	void setDebugMode(bool debug);
	bool toggleDebug() { setDebugMode(!debug); return debug; }
	bool isDebug() { return debug; }

	void setWindowFocused(bool windowFocused);
	bool isWindowFocused() { return windowFocused; }

	bool shoulResetMouse();
	void setResetMouse(bool resetMouse);

};


