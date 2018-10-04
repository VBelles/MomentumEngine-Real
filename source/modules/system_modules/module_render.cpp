#include "mcv_platform.h"
#include "module_render.h"
#include "modules/module.h"
#include "windows/app.h"
#include "imgui/imgui_impl_dx11.h"
#include "render/render_objects.h"
#include "render/render_utils.h"
#include "render/render_manager.h"
#include "components/comp_light_dir.h"
#include "render/texture/material.h"
#include "render/texture/texture.h"
#include "resources/json_resource.h"
#include "skeleton/game_core_skeleton.h"
#include "camera/camera.h"
#include "geometry/curve.h"
#include "geometry/rigid_anim.h"
#include "render/mesh/collision_mesh.h" 
#include "components/postfx/comp_render_focus.h"
#include "components/postfx/comp_render_blur.h"
#include "components/postfx/comp_render_blur_radial.h"
#include "components/postfx/comp_color_grading.h"
#include "components/postfx/comp_render_outlines.h"
#include "components/postfx/comp_render_bloom.h"
#include "components/postfx/comp_render_fog.h"
#include "components/postfx/comp_render_fxaa.h"
#include "components/postfx/comp_render_hsv.h"
#include "components/postfx/comp_render_vignette.h"

CModuleRender::CModuleRender(const std::string& name)
	: IModule(name) {
}

// All techs are loaded from this json file
bool parseTechniques() {
	json j = loadJson("data/techniques.json");
	for (auto it = j.begin(); it != j.end(); ++it) {

		std::string tech_name = it.key() + ".tech";
		json& tech_j = it.value();

		CRenderTechnique* tech = new CRenderTechnique();
		if (!tech->create(tech_name, tech_j)) {
			fatal("Failed to create tech '%s'\n", tech_name.c_str());
			return false;
		}
		Resources.registerResource(tech);
	}
	return true;
}

bool CModuleRender::start() {
	if (!Render.createDevice(_xres, _yres, vsync)) return false;

	if (!CVertexDeclManager::get().create()) return false;

	// Register the resource types
	Resources.registerResourceClass(getResourceClassOf<CJsonResource>());
	Resources.registerResourceClass(getResourceClassOf<CTexture>());
	Resources.registerResourceClass(getResourceClassOf<CRenderMesh>());
	Resources.registerResourceClass(getResourceClassOf<CRenderTechnique>());
	Resources.registerResourceClass(getResourceClassOf<CMaterial>());
	Resources.registerResourceClass(getResourceClassOf<CCurve>());
	Resources.registerResourceClass(getResourceClassOf<CGameCoreSkeleton>());
	Resources.registerResourceClass(getResourceClassOf<CCollisionMesh>());
	Resources.registerResourceClass(getResourceClassOf<RigidAnims::CRigidAnimResource>());

	if (!createRenderObjects()) return false;

	if (!createRenderUtils()) return false;

	// Main render target before rendering in the backbuffer
	rt_main = new CRenderToTexture;
	if (!rt_main->createRT("rt_main.dds", Render.width, Render.height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN, true))
		return false;

	if (!deferred.create(Render.width, Render.height))
		return false;

	setBackgroundColor(0.0f, 0.125f, 0.3f, 1.f);

	//Render splash screen
	json j = loadJson("data/dump_texture_tech.json");
	std::string tech_name = "dump_texture.tech";
	json& tech_j = j;
	CRenderTechnique* tech = new CRenderTechnique();
	if (!tech->create(tech_name, tech_j)) {
		fatal("Failed to create tech '%s'\n", tech_name.c_str());
		return false;
	}
	Resources.registerResource(tech);
	Render.startRenderInBackbuffer();
	renderFullScreenQuad("dump_texture.tech", Resources.get("data/textures/gui/splash_screen.dds")->as<CTexture>());
	Render.swapChain->Present(0, 0);

	// --------------------------------------------
	// ImGui
	auto& app = CApp::get();
	if (!ImGui_ImplDX11_Init(app.getWnd(), Render.device, Render.ctx))
		return false;

	if (!parseTechniques()) return false;

	// Some camera in case there is no camera in the scene
	camera.lookAt(VEC3(12.0f, 8.0f, 8.0f), VEC3::Zero, VEC3::UnitY);
	camera.setPerspective(60.0f * 180.f / (float)M_PI, 0.1f, 1000.f);

	if (!cb_camera.create(CB_CAMERA))      return false;
	if (!cb_object.create(CB_OBJECT))      return false;
	if (!cb_light.create(CB_LIGHT))        return false;
	if (!cb_globals.create(CB_GLOBALS))    return false;
	if (!cb_blur.create(CB_BLUR))          return false;
	if (!cb_gui.create(CB_GUI))            return false;
	if (!cb_fog.create(CB_FOG))            return false;
	if (!cb_particles.create(CB_PARTICLE)) return false;
	if (!cb_slash.create(CB_SLASH))		   return false;


	cb_globals.global_exposure_adjustment = 1.f;
	cb_globals.global_ambient_adjustment = 0.5f;
	cb_globals.global_shadow_adjustment = 0.1f;
	cb_globals.global_world_time = 0.f;
	cb_globals.global_hdr_enabled = 1.f;
	cb_globals.global_gamma_correction_enabled = 1.f;
	cb_globals.global_tone_mapping_mode = 1.f;
	cb_globals.global_hue_adjustment = 1.f;
	cb_globals.global_saturation_adjustment = 1.f;
	cb_globals.global_brightness_adjustment = -0.9f;
	cb_globals.global_contrast_adjustment = 0.215f;

	cb_light.activate();
	cb_object.activate();
	cb_camera.activate();
	cb_globals.activate();
	cb_blur.activate();
	cb_gui.activate();
	cb_fog.activate();
	cb_particles.activate();
	cb_slash.activate();

	//activateMainCamera();

	return true;
}

// Forward the OS msg to the IMGUI
LRESULT CModuleRender::OnOSMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}


bool CModuleRender::stop() {
	cb_light.destroy();
	cb_camera.destroy();
	cb_object.destroy();
	cb_globals.destroy();
	cb_blur.destroy();
	cb_gui.destroy();
	cb_fog.destroy();
	cb_particles.destroy();
	cb_slash.destroy();

	ImGui_ImplDX11_Shutdown();

	destroyRenderUtils();
	destroyRenderObjects();

	Resources.destroyAll();

	Render.destroyDevice();
	return true;
}

void CModuleRender::update(float delta) {
	cb_globals.global_world_time += delta;
}

void CModuleRender::render() {
	// Notify ImGUI that we are starting a new frame
	ImGui_ImplDX11_NewFrame();
	if (CApp::get().isDebug()) {
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		static int nframes = 5;
		ImGui::DragInt("NumFrames To Capture", &nframes, 0.1f, 1, 20);
		if (ImGui::SmallButton("Start CPU Trace Capturing")) {
			PROFILE_SET_NFRAMES(nframes);
		}
		if (ImGui::TreeNode("Render Control")) {
			ImGui::DragFloat("Exposure Adjustment", &cb_globals.global_exposure_adjustment, 0.01f, 0.01f, 32.f);
			ImGui::DragFloat("Ambient Adjustment", &cb_globals.global_ambient_adjustment, 0.01f, 0.0f, 1.f);
			ImGui::DragFloat("Shadow Adjustment", &cb_globals.global_shadow_adjustment, 0.01f, 0.0f, 1.f);
			ImGui::DragFloat("HDR", &cb_globals.global_hdr_enabled, 0.01f, 0.0f, 1.f);
			ImGui::DragFloat("Gamma Correction", &cb_globals.global_gamma_correction_enabled, 0.01f, 0.0f, 1.f);
			ImGui::DragFloat("Reinhard vs Uncharted2", &cb_globals.global_tone_mapping_mode, 0.01f, 0.0f, 1.f);
			ImGui::DragFloat("Hue Adjustment", &cb_globals.global_hue_adjustment, 0.01f, -10.0f, 10.f);
			ImGui::DragFloat("Saturation Adjustment", &cb_globals.global_saturation_adjustment, 0.01f, -10.0f, 10.f);
			ImGui::DragFloat("Brightness Adjustment", &cb_globals.global_brightness_adjustment, 0.01f, -10.0f, 10.f);
			ImGui::DragFloat("Contrast Adjustment", &cb_globals.global_contrast_adjustment, 0.01f, -10.0f, 10.f);
			ImGui::DragFloat("Skybox Ratio", &cb_globals.global_skybox_ratio, 0.01f, 0.0f, 1.f);

			// Must be in the same order as the RO_* ctes
			static const char* render_output_str =
				"Complete\0"
				"Albedo\0"
				"Normal\0"
				"Roughness\0"
				"Metallic\0"
				"World Pos\0"
				"Depth Linear\0"
				"Ambient Occlusion\0"
				"\0";
			ImGui::Combo("Output", &cb_globals.global_render_output, render_output_str);

			ImGui::TreePop();
		}
	}
}

void CModuleRender::configure(int xres, int yres, bool vsync) {
	_xres = xres;
	_yres = yres;
	this->vsync = vsync;
}

void CModuleRender::setBackgroundColor(float r, float g, float b, float a) {
	_backgroundColor.x = r;
	_backgroundColor.y = g;
	_backgroundColor.z = b;
	_backgroundColor.w = a;
}

bool CModuleRender::resizeWindow() {
	if (!Render.resize(_xres, _yres)) return false;

	// --------------------------------------------
	// ImGui
	auto& app = CApp::get();
	if (!ImGui_ImplDX11_Init(app.getWnd(), Render.device, Render.ctx))
		return false;

	// Main render target before rendering in the backbuffer
	rt_main = new CRenderToTexture;
	if (!rt_main->createRT("rt_main.dds", Render.width, Render.height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN, true))
		return false;

	if (!deferred.create(Render.width, Render.height))
		return false;

	return true;
}

// -------------------------------------------------
void CModuleRender::activateMainCamera() {
	CCamera* cam = &camera;

	std::string cameraName = freeCamera ? DEBUG_CAMERA : GAME_CAMERA;
	h_e_camera = getEntityByName(cameraName);
	if (h_e_camera.isValid()) {
		CEntity* e_camera = h_e_camera;
		TCompCamera* c_camera = e_camera->get< TCompCamera >();
		cam = c_camera->getCamera();
		assert(cam);
		CRenderManager::get().setEntityCamera(h_e_camera);
	}
	activateCamera(*cam, Render.width, Render.height);
}

void CModuleRender::generateFrame() {
	{
		CTraceScoped gpu_scope("Frame");
		PROFILE_FUNCTION("CModuleRender::generateFrame");

		activateMainCamera();
		cb_globals.updateGPU();

		if (h_e_camera.isValid()) {
			deferred.render(rt_main, h_e_camera);
		}

		CRenderManager::get().renderCategory("slash");
		CRenderManager::get().renderCategory("particles");
		CRenderManager::get().renderCategory("distorsions");
		CRenderManager::get().renderCategory("textured");
		CRenderManager::get().renderCategory("general");

		{
			PROFILE_FUNCTION("Modules");
			CTraceScoped gpu_scope("Modules");
			EngineModules.render();
		}

		// Apply postFX
		CTexture* curr_rt = rt_main;
		if (h_e_camera.isValid()) {
			CEntity* e_cam = h_e_camera;

			// The bloom blurs the given input
			TCompRenderBloom* c_render_bloom = e_cam->get< TCompRenderBloom >();
			if (c_render_bloom) {
				c_render_bloom->generateHighlights(rt_main, deferred.rt_self_illum);
				c_render_bloom->addBloom();
			}

			EngineModules.renderAfterBloom();

			// Check if we have a render_fog component
			TCompRenderFog* c_render_fog = e_cam->get< TCompRenderFog >();
			if (c_render_fog)
				curr_rt = c_render_fog->apply(curr_rt);

			// Check if we have a render_fx component
			TCompRenderBlur* c_render_blur = e_cam->get< TCompRenderBlur >();
			if (c_render_blur)
				curr_rt = c_render_blur->apply(curr_rt);

			// Check if we have a render_fx component
			TCompRenderBlurRadial* c_render_blur_radial = e_cam->get< TCompRenderBlurRadial >();
			if (c_render_blur_radial)
				curr_rt = c_render_blur_radial->apply(curr_rt);

			// Requires the blur component to be active
			TCompRenderFocus* c_render_focus = e_cam->get< TCompRenderFocus >();
			if (c_render_focus)
				curr_rt = c_render_focus->apply(rt_main, curr_rt);

			// Check if we have a color grading component
			TCompColorGrading* c_color_grading = e_cam->get< TCompColorGrading >();
			if (c_color_grading)
				curr_rt = c_color_grading->apply(curr_rt);

			TCompRenderOutlines* c_render_outlines = e_cam->get< TCompRenderOutlines >();
			if (c_render_outlines)
				c_render_outlines->apply();

			// Check if we have a render_fxaa component
			TCompRenderFxaa* c_render_fxaa = e_cam->get< TCompRenderFxaa >();
			if (c_render_fxaa)
				curr_rt = c_render_fxaa->apply(curr_rt);

			// Check if we have a render_hsv component
			TCompRenderHsv* c_render_hsv = e_cam->get< TCompRenderHsv >();
			if (c_render_hsv)
				curr_rt = c_render_hsv->apply(curr_rt);

			// Check if we have a render_vignette component
			TCompRenderVignette* c_render_vignette = e_cam->get< TCompRenderVignette >();
			if (c_render_vignette)
				curr_rt = c_render_vignette->apply(curr_rt);
		}

		Render.startRenderInBackbuffer();

		renderFullScreenQuad("dump_texture.tech", curr_rt);
	}
	{
		PROFILE_FUNCTION("GUI");
		CTraceScoped gpu_scope("GUI");

		activateRSConfig(RSCFG_CULL_NONE);
		activateZConfig(ZCFG_DISABLE_ALL);
		activateBlendConfig(BLEND_CFG_COMBINATIVE);

		activateCamera(EngineGUI.getCamera(), Render.width, Render.height);
		EngineModules.renderGUI();

		activateRSConfig(RSCFG_DEFAULT);
		activateZConfig(ZCFG_DEFAULT);
		activateBlendConfig(BLEND_CFG_DEFAULT);
	}
	{
		PROFILE_FUNCTION("ImGui::Render");
		CTraceScoped gpu_scope("ImGui");
		ImGui::Render();
	}
	// Present the information rendered to the back buffer to the front buffer (the screen)
	{
		PROFILE_FUNCTION("Render.swapChain");
		mtx.lock();
		if (vsync) {
			Render.swapChain->Present(1, 0);
		}
		else {
			Render.swapChain->Present(0, 0);
		}
		mtx.unlock();
	}
}


CHandle CModuleRender::getCurrentCameraHandle() {
	return h_e_camera;
}

bool CModuleRender::toggleFreeCamera() {
	freeCamera = !freeCamera;
	return freeCamera;
}
