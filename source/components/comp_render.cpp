#include "mcv_platform.h"
#include "comp_render.h"

DECL_OBJ_MANAGER("render", TCompRender);

#include "render/render_objects.h"
#include "render/texture/texture.h"
#include "render/texture/material.h"

void TCompRender::debugInMenu() {
	ImGui::ColorEdit4("Color", &color.x);
}

void TCompRender::loadMesh(const json& j, TEntityParseContext& ctx) {

	std::string name_mesh = j.value("mesh", "axis.mesh");
	mesh = Resources.get(name_mesh)->as<CRenderMesh>();

	if (j.count("materials")) {
		auto& j_mats = j["materials"];
		assert(j_mats.is_array());
		for (size_t i = 0; i < j_mats.size(); ++i) {
			std::string name_material = j_mats[i];
			const CMaterial* material = Resources.get(name_material)->as<CMaterial>();
			materials.push_back(material);
		}
	}
	else {
		const CMaterial* material = Resources.get("data/materials/solid.material")->as<CMaterial>();
		materials.push_back(material);
	}

	// If there is a color in the json, read it
	if (j.count("color")) {
		color = loadVEC4(j["color"]);
	}
	originalColor = color;
}

void TCompRender::load(const json& j, TEntityParseContext& ctx) {

	// We expect an array of things to render: mesh + materials, mesh + materials, ..
	if (j.is_array()) {
		for (size_t i = 0; i < j.size(); ++i)
			loadMesh(j[i], ctx);
	}
	else {
		// We accept not receiving an array of mesh inside the comp_render, for handle files
		loadMesh(j, ctx);
	}

}

void TCompRender::setMesh(std::string meshName) {
	mesh = Resources.get(meshName)->as<CRenderMesh>();
}

void TCompRender::update(float delta) {
	if (isColorChanged && timer.elapsed() >= timeToNormal) {
		color = originalColor;
		isColorChanged = false;
	}
}

void TCompRender::TurnRed(float time) {
	timer.reset();
	timeToNormal = time;
	isColorChanged = true;
	color = VEC4(1, 0, 0, 1);
}
