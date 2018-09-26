#ifndef INC_COMP_GROUP_H_
#define INC_COMP_GROUP_H_


class CTransform;

struct TCompGroup : public TCompBase {

	VHandles handles;

	void debugInMenu();
	void add(CHandle h_new_child);
	~TCompGroup();
};

#endif
