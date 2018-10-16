#include "mcv_platform.h"
#include "comp_screen_transition.h"
#include "render/render_objects.h"
#include "resources/resources_manager.h"

DECL_OBJ_MANAGER("screen_transition", TCompScreenTransition);

void TCompScreenTransition::debugInMenu() {
	ImGui::DragFloat("Speed", &speed, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Ratio", &ratio, 0.01f, 0.f, 1.f);
	ImGui::DragFloat("Target ratio", &targetRatio, 0.01f, 0.f, 1.f);
	((CTexture*)texture)->debugInMenu();
	((CTexture*)transitionTexture)->debugInMenu();
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
}

void TCompScreenTransition::update(float dt) {
	if (transitioning) {
		if (ratio < targetRatio) {
			ratio = clamp(ratio + speed * dt, 0.f, 1.f);
		}
		else if (ratio > targetRatio) {
			ratio = clamp(ratio - speed * dt, 0.f, 1.f);
		}
		else {
			transitioning = false;
		}
	}
}

CTexture* TCompScreenTransition::apply(CTexture * in_texture) {
	if (!transitioning) {
		return in_texture;
	}

	CTraceScoped scope("CompScreenTransition");

	cb_globals.global_shared_fx_amount = ratio;
	cb_globals.updateGPU();

	rt->activateRT();
	in_texture->activate(TS_ALBEDO);
	texture->activate(TS_NORMAL);
	transitionTexture->activate(TS_METALLIC);

	tech->activate();
	mesh->activateAndRender();

	return rt;
}

void TCompScreenTransition::startTransition() {
	ratio = 0.f;
	targetRatio = 1.f;
	transitioning = true;
}

void TCompScreenTransition::startTransition(float startingRatio, float finalRatio) {
	ratio = clamp(startingRatio, 0.f, 1.f);
	targetRatio = clamp(finalRatio, 0.f, 1.f);
	transitioning = true;
}

bool TCompScreenTransition::isTransitioning() {
	return transitioning;
}