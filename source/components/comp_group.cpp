#include "mcv_platform.h"
#include "comp_group.h"

DECL_OBJ_MANAGER("group", TCompGroup);

void TCompGroup::debugInMenu() {
	for (auto h : handles)
		h.debugInMenu();
}

TCompGroup::~TCompGroup() {
	for (auto h : handles)
		h.destroy();
}

void TCompGroup::add(CHandle h_new_child) {
	handles.push_back(h_new_child);
	// The new child should not have another parent. Or we should tell him he is no longer his child...

    // Problema: Si una entity usa un prefab que contiene m�s de una entity, ya tendr� group_root_etc como owner.
    //if (h_new_child.getOwner().isValid()) return;

	//assert(!h_new_child.getOwner().isValid());
	// Using the handle owner to set his parent my entity
	h_new_child.setOwner(CHandle(this).getOwner());
}
