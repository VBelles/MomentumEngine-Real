#pragma once

#include "modules/module.h"
#include "camera/camera.h"
#include "handle/handle_def.h"
#include "render/deferred_renderer.h"

class CModuleRender : public IModule {
private:
	CDeferredRenderer deferred;
	CRenderToTexture* rt_main = nullptr;

	CHandle h_e_camera;
	CCamera camera;

	int _xres = 0;
	int _yres = 0;
	bool vsync = false;

	VEC4 _backgroundColor;

	bool freeCamera = false;

public:
	CModuleRender(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

	void generateFrame();
	void activateMainCamera();

	void configure(int xres, int yres, bool vsync);
	void setBackgroundColor(float r, float g, float b, float a);
	bool resizeWindow();

	LRESULT OnOSMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

	CHandle getCurrentCameraHandle();
	bool toggleFreeCamera();
	bool isFreeCamera();
};
