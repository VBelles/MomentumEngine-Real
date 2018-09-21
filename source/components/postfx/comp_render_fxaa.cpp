#include "mcv_platform.h"
#include "comp_render_fxaa.h"
#include "render/render_objects.h"
#include "resources/resources_manager.h"

DECL_OBJ_MANAGER("render_fxaa", TCompRenderFxaa);

void TCompRenderFxaa::debugInMenu() {
	ImGui::Checkbox("Enabled", &enabled);
}


void TCompRenderFxaa::load(const json& j, TEntityParseContext& ctx) {
	
	if (!rt) {
		rt = new CRenderToTexture;
		// Create a unique name for the rt 
		char rt_name[64];
		sprintf(rt_name, "Fxaa_%08x", CHandle(this).asUnsigned());
		bool is_ok = rt->createRT(rt_name, Render.width, Render.height, DXGI_FORMAT_B8G8R8A8_UNORM);
		assert(is_ok);
	}
	tech = Resources.get("fxaa.tech")->as<CRenderTechnique>();
	mesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();

	setEnable(j.value("enabled", false));
}

CTexture* TCompRenderFxaa::apply(CTexture * in_texture) {
	if (!enabled)
		return in_texture;
	CTraceScoped scope("CompFxaa");

	rt->activateRT();
	in_texture->activate(TS_ALBEDO);

	tech->activate();
	mesh->activateAndRender();

	return rt;
}

void TCompRenderFxaa::setEnable(bool enabled) {
	this->enabled = enabled;
}
