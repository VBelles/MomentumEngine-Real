#ifndef INC_MCV_PLATFORM_
#define INC_MCV_PLATFORM_


// Global settings
#define _CRT_SECURE_NO_WARNINGS     // Don't warn about using fopen..
#define WIN32_LEAN_AND_MEAN         // Reduce the size of things included in windows.h
#define _USE_MATH_DEFINES           // M_PI M_PI_2

// Request dedicated graphics
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;	// Dedicated graphics for NVIDIA
}
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;		// Dedicated graphics for AMD
}

// C/C++
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstdarg>
#include <cstdint>        // uint32_t
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <locale>
#include <fstream>


// Windows/OS Platform
#define NOMINMAX                    // To be able to use std::min without windows problems
#include <windows.h>
#include <d3d11.h>

//Physx
#include <PxPhysicsAPI.h>

//Cal3D
#include <Cal3D/cal3d.h>

//FMOD
#include <fmod.hpp>
#include <fmod_studio.hpp>

// Engine
#include "utils/json.hpp"
using json = nlohmann::json;

#include "windows/windowsUtils.h"

#include "profiling/profiling.h"
#include "geometry/geometry.h"
#include "render/render.h"
#include "render/shaders/vertex_declarations.h"
#include "utils/utils.h"
#include "utils/timer.h"
#include "utils/timer2.h"
#include "utils/physx_utils.h"
#include "imgui/imgui.h"
#include "camera/camera.h"
#include "engine.h"
#include "resources/resources_manager.h"
#include "windows/app.h"
#include "game_constants.h"

// Components
#include "components/comp_base.h"
#include "components/comp_render.h"
#include "components/comp_collider.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "skeleton/comp_skeleton.h"

#include "modules/system_modules/sound/music_player.h"

#endif
