#pragma once

#include "modules/module.h"
#include "camera/camera.h"
#include "handle/handle_def.h"
#include "render/deferred_renderer.h"

class CModuleRender : public IModule {
    CDeferredRenderer deferred;
    CRenderToTexture* rt_main = nullptr;
public:
    CModuleRender(const std::string& name);
    bool start() override;
    bool stop() override;
    void update(float delta) override;
    void render() override;

    void generateFrame();
    void activateMainCamera();

    void configure(int xres, int yres);
    void setBackgroundColor(float r, float g, float b, float a);

    LRESULT OnOSMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

	CHandle getCurrentCameraHandle() { return h_e_camera; }

	bool toggleFreeCamera() { freeCamera = !freeCamera; return freeCamera; }

private:
    CHandle h_e_camera;
    CCamera camera;

    int _xres;
    int _yres;
    VEC4 _backgroundColor;

	bool freeCamera = false;
};
