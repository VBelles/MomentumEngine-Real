#include "mcv_platform.h"
#include "entity_parser.h"
#include "entity.h"
#include "common_msgs.h"
#include "resources/json_resource.h"
#include "components/comp_group.h"
#include "components/comp_name.h"

// Find in the current list of entities created, the first entity matching
// the given name
CHandle TEntityParseContext::findEntityByName(const std::string& name) const {
	// Search linearly in the list of entity currently loaded
	for (auto h : entities_loaded) {
		CEntity* e = h;
		if (e->getName() == name)
			return h;
	}

	// Delegate it to my parent
	if (parent) return parent->findEntityByName(name);

	return getEntityByName(name);
}

TEntityParseContext::TEntityParseContext(TEntityParseContext& another_ctx, const CTransform& delta_transform) {
	parent = &another_ctx;
	recursion_level = another_ctx.recursion_level + 1;
	entity_starting_the_parse = another_ctx.entity_starting_the_parse;
	root_transform = another_ctx.root_transform.combineWith(delta_transform);
	//VEC3 p = root_transform.getPosition(); float y, pitch; root_transform.getYawPitchRoll(&y, &pitch);
	//dbg("New root transform is Pos:%f %f %f Yaw: %f\n", p.x, p.y, p.z, rad2deg(y));
}

bool parseScene(const std::string& filename, TEntityParseContext& ctx) {
	ctx.filename = filename;

	const json& j_scene = Resources.get(filename)->as<CJsonResource>()->getJson();
	assert(j_scene.is_array());

	// For each item in the array...
	for (int i = 0; i < j_scene.size(); ++i) {
		auto& j_item = j_scene[i];

		assert(j_item.is_object());

		if (j_item.count("entity")) {
			auto& j_entity = j_item["entity"];

			CHandle h_e;

			// Do we have the prefab key in the json?
			if (j_entity.count("prefab")) {

				// Get the src/id of the prefab
				std::string prefab_src = j_entity["prefab"];
				assert(!prefab_src.empty());

				// Get delta transform where we should instantiate this transform
				CTransform delta_transform;
				if (j_entity.count("transform"))
					delta_transform.load(j_entity["transform"]);

				// Parse the prefab, if any other child is created they will inherit our ctx transform
				TEntityParseContext prefab_ctx(ctx, delta_transform);
				prefab_ctx.parsing_prefab = true;
				if (!parseScene(prefab_src, prefab_ctx))
					return false;

				assert(!prefab_ctx.entities_loaded.empty());

				// Create a new fresh entity
				h_e = prefab_ctx.entities_loaded[0];

				// Cast to entity object
				CEntity* e = h_e;

				// We give an option to 'reload' the prefab by modifying existing components, 
				// like changing the name, add other components, etc, but we don't want to parse again 
				// the comp_transform, because it was already parsed as part of the root
				// As the json is const as it's a resouce, we make a copy of the prefab section and
				// remove the transform
				json j_entity_without_transform = j_entity;
				j_entity_without_transform.erase("transform");

				// Do the parse now outside the 'prefab' context
				prefab_ctx.parsing_prefab = false;
				e->load(j_entity_without_transform, prefab_ctx);

			}
			else {
				// Create a new fresh entity
				h_e.create< CEntity >();

				// Cast to entity object
				CEntity* e = h_e;

				// Do the parse
				e->load(j_entity, ctx);

			}
			ctx.entities_loaded.push_back(h_e);
		}
	}

	// Create a comp_group automatically if there is more than one entity
	if (ctx.entities_loaded.size() > 1) {
		//Create new entity for group root
		CHandle groupRootEntity;
		groupRootEntity.create<CEntity>();
		CEntity* e_root_of_group = groupRootEntity;
		assert(e_root_of_group);

		// Create a new instance of the TCompGroup
		CHandle h_group = getObjectManager<TCompGroup>()->createHandle();
		// Add it to the entity
		e_root_of_group->set(h_group.getType(), h_group);

		// Add name component
		TCompName* nameComp = getObjectManager<TCompName>()->createHandle();
		nameComp->setName(("group_root_" + filename).c_str());
		e_root_of_group->set(CHandle(nameComp).getType(), nameComp);

		// Now add the rest of entities
		TCompGroup* c_group = h_group;
		for (size_t i = 0; i < ctx.entities_loaded.size(); ++i) {
			c_group->add(ctx.entities_loaded[i]);
		}
	}

	// Notify each entity created that we have finished
	// processing this file
	if (!ctx.parsing_prefab) {
		TMsgEntitiesGroupCreated msg = { ctx };
		for (auto h : ctx.entities_loaded)
			h.sendMsg(msg);
	}
	return true;
}
