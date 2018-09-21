#pragma once

#include "components/comp_base.h"

class CTexture;
class CRenderToTexture;
class CRenderMesh;

class TCompRenderVignette: public TCompBase {
private:
	const CRenderTechnique* tech = nullptr;
	const CRenderMesh* mesh = nullptr;
	CRenderToTexture* rt = nullptr;

	bool enabled = false;

public:
	DECL_SIBLING_ACCESS();

    void debugInMenu();
    void load(const json& j, TEntityParseContext& ctx);

	CTexture* apply(CTexture* in_texture);
	void setEnable(bool enabled);
};
