#include "mcv_platform.h"
#include "module_slash.h"
#include "modules/system_modules/slash/comp_slash.h"
#include "components/comp_transform.h"

CModuleSlash::CModuleSlash(const std::string& name)
	: IModule(name) {
}

bool CModuleSlash::start() {
	return true;
}

bool CModuleSlash::stop() {
	return true;
}

void CModuleSlash::update(float delta) {
}

void CModuleSlash::render() {
	const CRenderTechnique* technique = Resources.get("slash.tech")->as<CRenderTechnique>();
	assert(technique);
	technique->activate();
	getObjectManager<TCompSlash>()->forEach([](TCompSlash* slashComp) {
		if (!slashComp->texture || !slashComp->mesh) return;

		slashComp->texture->activate(TS_ALBEDO);

		TCompTransform* transform = slashComp->get<TCompTransform>();
		cb_object.obj_world = transform->asMatrix();
		cb_object.obj_color = slashComp->color;
		cb_object.updateGPU();

		cb_slash.slash_duration = slashComp->duration;
		cb_slash.slash_fadeout_multiplier = slashComp->fadeoutMultiplier;
		cb_slash.updateGPU();

		slashComp->mesh->activateAndRender();
	});

}


