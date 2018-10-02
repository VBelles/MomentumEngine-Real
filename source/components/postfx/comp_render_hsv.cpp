#include "mcv_platform.h"
#include "comp_render_hsv.h"
#include "render/render_objects.h"
#include "resources/resources_manager.h"

DECL_OBJ_MANAGER("render_hsv", TCompRenderHsv);

void TCompRenderHsv::debugInMenu() {
	ImGui::Checkbox("Enabled", &enabled);
}


void TCompRenderHsv::load(const json& j, TEntityParseContext& ctx) {
	
	if (!rt) {
		rt = new CRenderToTexture;
		// Create a unique name for the rt 
		char rt_name[64];
		sprintf(rt_name, "Hsv_%08x", CHandle(this).asUnsigned());
		bool is_ok = rt->createRT(rt_name, Render.width, Render.height, DXGI_FORMAT_B8G8R8A8_UNORM);
		assert(is_ok);
	}
	tech = Resources.get("hsv.tech")->as<CRenderTechnique>();
	mesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();

	setEnable(j.value("enabled", false));
}

CTexture* TCompRenderHsv::apply(CTexture * in_texture) {
	if (!enabled)
		return in_texture;
	CTraceScoped scope("CompHsv");

	rt->activateRT();
	in_texture->activate(TS_ALBEDO);

	tech->activate();
	mesh->activateAndRender();

	return rt;
}

void TCompRenderHsv::setEnable(bool enabled) {
	this->enabled = enabled;
}
