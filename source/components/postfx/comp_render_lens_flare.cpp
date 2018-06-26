#include "mcv_platform.h"
#include "comp_render_lens_flare.h"
#include "render/render_objects.h"
#include "resources/resources_manager.h"

DECL_OBJ_MANAGER("render_lens_flare", TCompRenderLensFlare);

void TCompRenderLensFlare::debugInMenu() {
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::DragFloat4("Color", &color.x, 0.001f, 0.0f, 1.0f);
	ImGui::DragInt("Samples", &samples, 0.0001f, 0.0f, 1.0f);
	ImGui::DragFloat("Texture scale", &texScale, 0.0001f, 0.0f, 1.0f);
	ImGui::DragFloat("Blur scale", &blurScale, 0.001f, 0.0f, 1.0f);
	int samples;
	float texScale;
	float blurScale;
}

void TCompRenderLensFlare::registerMsgs() {
  
}

void TCompRenderLensFlare::load(const json& j, TEntityParseContext& ctx) {

	color = j.count("color") ? loadVEC4(j["color"]) : color;
	samples = j.value("samples", samples);
	texScale = j.value("tex_scale", texScale);
	blurScale = j.value("blur_scale", blurScale);
	
	if (!rt) {
		rt = new CRenderToTexture;
		// Create a unique name for the rt 
		char rt_name[64];
		sprintf(rt_name, "LensFlare_%08x", CHandle(this).asUnsigned());
		bool is_ok = rt->createRT(rt_name, Render.width, Render.height, DXGI_FORMAT_B8G8R8A8_UNORM);
		assert(is_ok);
	}
	tech = Resources.get("lens_flare.tech")->as<CRenderTechnique>();
	mesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();

	setEnable(j.value("enabled", false));

}

CTexture* TCompRenderLensFlare::apply(CTexture * in_texture) {
	if (!enabled)
		return in_texture;
	CTraceScoped scope("CompLensFlare");

	rt->activateRT();
	in_texture->activate(TS_ALBEDO);

	cb_lens_flare.lensFlareColor = color;
	cb_lens_flare.lensFlareSamples = samples;
	cb_lens_flare.lensFlareTexScale = texScale;
	cb_lens_flare.lensFlareBlurScale = blurScale;
	cb_fog.activate();
	cb_fog.updateGPU();

	tech->activate();
	mesh->activateAndRender();

	return rt;
}

void TCompRenderLensFlare::setEnable(bool enabled) {
	this->enabled = enabled;
}
