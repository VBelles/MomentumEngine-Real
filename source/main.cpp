#include "mcv_platform.h"
#include "windows/app.h"
#include <SLB/lua.hpp>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

    CApp app;

    if (!app.readConfig())
        return -1;

    if (!app.createWindow(hInstance, nCmdShow))
        return -2;

    if (!app.start())
        return -3;

    app.mainLoop();

    app.stop();

    return 0;
}
