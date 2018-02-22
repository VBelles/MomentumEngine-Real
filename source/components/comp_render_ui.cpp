#include "mcv_platform.h"
#include "comp_render_ui.h"

DECL_OBJ_MANAGER("render_ui", TCompRenderUI);

//void TCompRenderUI::debugInMenu() { }

void TCompRenderUI::load(const json& j, TEntityParseContext& ctx) {

}

void TCompRenderUI::renderUI() {
	
	for (auto& render : toRender) {
		render();
	}
}

void TCompRenderUI::registerOnRenderUI(std::function<void()> renderUi) {
	toRender.push_back(renderUi);
}
