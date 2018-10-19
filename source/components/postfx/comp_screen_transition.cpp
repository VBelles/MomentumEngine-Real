#include "mcv_platform.h"
#include "comp_screen_transition.h"
#include "render/render_objects.h"
#include "resources/resources_manager.h"

DECL_OBJ_MANAGER("screen_transition", TCompScreenTransition);

void TCompScreenTransition::debugInMenu() {
	ImGui::DragFloat("Speed", &speed, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Ratio", &ratio, 0.01f, 0.f, 1.f);
	ImGui::DragFloat("Target ratio", &targetRatio, 0.01f, 0.f, 1.f);
	ImGui::DragFloat("Alplha speed", &alphaSpeed, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Alpha", &alpha, 0.01f, 0.f, 1.f);
	ImGui::DragFloat("Target alpha", &targetAlpha, 0.01f, 0.f, 1.f);
	if (texture) ((CTexture*)texture)->debugInMenu();
	if (transitionTexture) ((CTexture*)transitionTexture)->debugInMenu();
}

void TCompScreenTransition::load(const json& j, TEntityParseContext& ctx) {
	if (!rt) {
		rt = new CRenderToTexture;
		// Create a unique name for the rt 
		char rt_name[64];
		sprintf(rt_name, "ScreenTransition_%08x", CHandle(this).asUnsigned());
		bool is_ok = rt->createRT(rt_name, Render.width, Render.height, DXGI_FORMAT_B8G8R8A8_UNORM);
		assert(is_ok);
	}
	tech = Resources.get("screen_transition.tech")->as<CRenderTechnique>();
	mesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();
	texture = Resources.get(j.value("texture", "data/textures/black.dds"))->as<CTexture>();
	transitionTexture = Resources.get(j.value("transition_texture", "data/textures/black.dds"))->as<CTexture>();
	speed = j.value("speed", speed);
	alphaSpeed = j.value("alpha_speed", alphaSpeed);
}

void TCompScreenTransition::update(float dt) {
	if (transitioning && timer.elapsed() >= waitTime) {
		if (alpha < targetAlpha) {
			alpha = clamp(alpha + alphaSpeed * dt, 0.f, targetAlpha);
		}
		else if (alpha > targetAlpha) {
			alpha = clamp(alpha - alphaSpeed * dt, targetAlpha, 1.f);
		}

		if (ratio < targetRatio) {
			ratio = clamp(ratio + speed * dt, 0.f, targetRatio);
		}
		else if (ratio > targetRatio) {
			ratio = clamp(ratio - speed * dt, targetRatio, 1.f);
		}
		else {
			transitioning = false;
		}
	}
}

void TCompScreenTransition::apply() {
	if (transitioning) {
		CTraceScoped scope("CompScreenTransition");

		cb_globals.global_shared_fx_amount = ratio;
		cb_globals.global_shared_fx_val1 = alpha;
		//cb_globals.global_shared_fx_val2 = 0;
		//cb_globals.global_shared_fx_val3 = 0;
		cb_globals.updateGPU();

		texture->activate(TS_ALBEDO);
		transitionTexture->activate(TS_NORMAL);

		tech->activate();
		mesh->activateAndRender();
	}
}

void TCompScreenTransition::startTransition(float delay) {
	waitTime = delay;
	timer.reset();
	ratio = 0.f;
	targetRatio = 1.f;
	alpha = 0.f;
	targetAlpha = 1.f;
	transitioning = true;
}

void TCompScreenTransition::startTransition(float startingRatio, float finalRatio, float delay) {
	waitTime = delay;
	timer.reset();
	ratio = clamp(startingRatio, 0.f, 1.f);
	targetRatio = clamp(finalRatio, 0.f, 1.f);
	alpha = clamp(startingRatio, 0.f, 1.f);
	targetAlpha = clamp(finalRatio, 0.f, 1.f);
	transitioning = true;
}

void TCompScreenTransition::startTransition(float startingRatio, float finalRatio, float startingAlpha, float finalAlpha, float delay) {
	waitTime = delay;
	timer.reset();
	ratio = clamp(startingRatio, 0.f, 1.f);
	targetRatio = clamp(finalRatio, 0.f, 1.f);
	alpha = clamp(startingAlpha, 0.f, 1.f);
	targetAlpha = clamp(finalAlpha, 0.f, 1.f);
	transitioning = true;
}

bool TCompScreenTransition::isTransitioning() {
	return transitioning;
}