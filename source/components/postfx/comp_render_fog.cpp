#include "mcv_platform.h"
#include "comp_render_fog.h"
#include "render/render_objects.h"
#include "resources/resources_manager.h"

DECL_OBJ_MANAGER("render_fog", TCompRenderFog);

void TCompRenderFog::debugInMenu() {
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::DragFloat3("Fog color", &fogColor.x, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("Fog density", &fogDensity, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("Fog max ratio", &fogMaxRatio, 0.001f, 0.0f, 1.0f);
}

void TCompRenderFog::registerMsgs() {
  
}

void TCompRenderFog::load(const json& j, TEntityParseContext& ctx) {

	fogColor = j.count("fog_color") ? loadVEC4(j["fog_color"]) : VEC4(0.8f, 0.8f, 0.8f, 1.f);
	fogDensity = j.value("fog_density", 0.005f);
	fogMaxRatio = j.value("fog_max_ratio", 1.f);
	
	if (!rt) {
		rt = new CRenderToTexture;
		// Create a unique name for the rt 
		char rt_name[64];
		sprintf(rt_name, "Fog_%08x", CHandle(this).asUnsigned());
		bool is_ok = rt->createRT(rt_name, Render.width, Render.height, DXGI_FORMAT_B8G8R8A8_UNORM);
		assert(is_ok);
	}
	tech = Resources.get("fog.tech")->as<CRenderTechnique>();
	mesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();

	setEnable(j.value("enabled", false));

}

CTexture* TCompRenderFog::apply(CTexture * in_texture) {
	if (!enabled)
		return in_texture;
	CTraceScoped scope("CompFog");

	rt->activateRT();
	in_texture->activate(TS_ALBEDO);

	cb_fog.fogColor = fogColor;
	cb_fog.fogDensity = fogDensity;
	cb_fog.fogMaxRatio = fogMaxRatio;
	cb_fog.activate();
	cb_fog.updateGPU();

	tech->activate();
	mesh->activateAndRender();

	return rt;
}

void TCompRenderFog::setEnable(bool enabled) {
	this->enabled = enabled;
}
