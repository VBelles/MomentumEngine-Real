#include "mcv_platform.h"
#include "module_entities.h"
#include "render/render_objects.h"
#include "render/texture/texture.h"
#include "render/texture/material.h"
#include "entity/entity.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_render_ui.h"
#include "components/comp_name.h"
#include "components/comp_tags.h"
#include "components/comp_light_dir.h"
#include "render/render_manager.h"
#include "components/comp_group.h"
#include "entity/msgs.h"

void CModuleEntities::loadListOfManagers(const json& j, std::vector< CHandleManager* > &managers) {
	managers.clear();
	// For each entry in j["update"] add entry to om_to_update
	std::vector< std::string > names = j;
	for (auto& n : names) {
		auto om = CHandleManager::getByName(n.c_str());
		assert(om || fatal("Can't find a manager of components of type %s to update. Check file components.json\n", n.c_str()));
		managers.push_back(om);
		om_to_update_active[om] = true;
	}
}

bool CModuleEntities::start() {
	json j = loadJson("data/components.json");

	// Initialize the ObjManager preregistered in their constructors
	// with the amount of components defined in the data/components.json
	std::map< std::string, int > comp_sizes = j["sizes"];;
	int default_size = comp_sizes["default"];

	// Reorder the init manager based on the json
	// The bigger the number in the init_order section, the lower comp_type id you get
	std::map< std::string, int > init_order = j["init_order"];;
	std::sort(CHandleManager::predefined_managers,
			  CHandleManager::predefined_managers + CHandleManager::npredefined_managers,
			  [&init_order](CHandleManager* m1, CHandleManager* m2) {
		int priority_m1 = init_order[m1->getName()];
		int priority_m2 = init_order[m2->getName()];
		return priority_m1 > priority_m2;
	});
	// Important that the entity is the first one for the chain destruction of components
	assert(strcmp(CHandleManager::predefined_managers[0]->getName(), "entity") == 0);

	// Now with the sorted array
	for (size_t i = 0; i < CHandleManager::npredefined_managers; ++i) {
		auto om = CHandleManager::predefined_managers[i];
		auto it = comp_sizes.find(om->getName());
		int sz = (it == comp_sizes.end()) ? default_size : it->second;
		dbg("Initializing obj manager %s with %d\n", om->getName(), sz);
		om->init(sz, false);
	}

	loadListOfManagers(j["update"], om_to_update);
	loadListOfManagers(j["render_debug"], om_to_render_debug);

	std::vector< std::string > names = j["multithread"];
	for (auto& n : names) {
		auto om = CHandleManager::getByName(n.c_str());
		assert(om || fatal("Can't find a manager of components of type %s to update. Check file components.json\n", n.c_str()));
		om->multithreaded = true;
	}

	return true;
}

void CModuleEntities::update(float delta) {
	float scaled_time = delta * time_scale_factor;

	for (auto om : om_to_update) {
		if (om_to_update_active[om]) {
			PROFILE_FUNCTION(om->getName());
			om->updateAll(scaled_time);
		}
	}

	CHandleManager::destroyAllPendingObjects();

	{ // Esto Juan lo tiene en module_render.cpp
		PROFILE_FUNCTION("CModuleRender::shadowsMapsGeneration");
		CTraceScoped gpu_scope("shadowsMapsGeneration");
		// Generate the shadow map for each active light
		getObjectManager<TCompLightDir>()->forEach([](TCompLightDir* c) {
			c->generateShadowMap();
		});
	}
}

bool CModuleEntities::stop() {
	// Destroy all entities, should destroy all components in chain
	auto hm = getObjectManager<CEntity>();
	hm->forEach([](CEntity* e) {
		CHandle h(e);
		h.destroy();
	});
	CHandleManager::destroyAllPendingObjects();
	all_registered_msgs.clear();
	TCompName::all_names.clear();
	return true;
}

void CModuleEntities::reset() {
	auto hm = getObjectManager<CEntity>();
	hm->forEach([](CEntity* e) {
		CHandle h(e);
		h.destroy();
	});
	CHandleManager::destroyAllPendingObjects();
	TCompName::all_names.clear();
}

void CModuleEntities::render() {
	if (CApp::get().isDebug()) {
		Resources.debugInMenu();

        ImGui::DragFloat("Time Factor", &time_scale_factor, 0.01f, 0.f, 1.0f);

		if (ImGui::TreeNode("All Entities...")) {
			ImGui::SameLine();
			static bool flat = true;
			ImGui::Checkbox("Flat", &flat);

			static ImGuiTextFilter Filter;
			ImGui::SameLine();
			Filter.Draw("Filter");

			auto om = getObjectManager<CEntity>();
			om->forEach([](CEntity* e) {
				CHandle h_e(e);
				if (!flat && h_e.getOwner().isValid())
					return;
				if (Filter.IsActive() && !Filter.PassFilter(e->getName()))
					return;
				ImGui::PushID(e);
				e->debugInMenu();
				ImGui::PopID();
			});
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("All Components...")) {
			for (uint32_t i = 1; i < CHandleManager::getNumDefinedTypes(); ++i)
				CHandleManager::getByType(i)->debugInMenuAll();
			ImGui::TreePop();
		}
		CTagsManager::get().debugInMenu();
	}

	// I just need to activate one light... but at this moment...
	getObjectManager<TCompLightDir>()->forEach([](TCompLightDir* c) {
		c->activate();
	});


	//static bool is_open = false;
	//ImGui::Checkbox("ImGui Demo", &is_open);
	//ImGui::ShowDemoWindow(&is_open);

	/*
	// ------------------------------------------
	// Do the basic render
	auto om_render = getObjectManager<TCompRender>();
	om_render->forEach([](TCompRender* c) {

	TCompTransform* c_transform = c->get<TCompTransform>();
	if (!c_transform)
	return;

	cb_object.obj_world = c_transform->asMatrix();
	cb_object.obj_color = c->color;
	cb_object.updateGPU();

	int idx = 0;
	c->mesh->activate();
	for (auto& m : c->materials) {
	if (m) {
	m->activate();
	c->mesh->renderSubMesh(idx);
	}
	++idx;
	}
	*/
	auto om_render_ui = getObjectManager<TCompRenderUI>();
	om_render_ui->forEach([](TCompRenderUI* c) {
		c->renderUI();
	});

	/*auto om_shadow = getObjectManager<TCompShadow>();
	om_shadow->forEach([](TCompShadow* c) {
	if (c->isEnabled()) {
	TCompTransform* c_transform = c->getTransform();
	if (!c_transform) return;

	cb_object.obj_world = c_transform->asMatrix();
	cb_object.updateGPU();

	for (auto& m : c->materials) {
	m->activate();
	}
	c->mesh->activateAndRender();
	}
	});*/

	//CRenderManager::get().renderCategory("default"); // Ya no hace falta.
	CRenderManager::get().debugInMenu();
	//renderDebugOfComponents();
}

// Shows render debug of all components (axis and so on).
void CModuleEntities::renderDebugOfComponents() {
	CTraceScoped gpu_scope("renderDebugOfComponents");
	PROFILE_FUNCTION("renderDebugOfComponents");
	// Change the technique to some debug solid
	auto solid = Resources.get("data/materials/solid.material")->as<CMaterial>();
	solid->activate();
	for (auto om : om_to_render_debug) {
		PROFILE_FUNCTION(om->getName());
		om->renderDebugAll();
	}
}

void  CModuleEntities::setManagerUpdate(std::string managerName, bool update) {
	auto om = CHandleManager::getByName(managerName.c_str());
	if (om_to_update_active.find(om) != om_to_update_active.end()) {
		om_to_update_active[om] = update;
	}
}