#pragma once

#include "components/comp_base.h"

class  CTexture;
class CRenderToTexture;
class CRenderMesh;

class TCompRenderLensFlare: public TCompBase {
private:
	const CRenderTechnique* tech = nullptr;
	const CRenderMesh* mesh = nullptr;
	CRenderToTexture* rt = nullptr;

	std::string lightSourceName;
	CHandle lightSource;
	VEC4 color = VEC4(1, 1, 1, 1);
	int samples = 4;
	float texScale = 1.f;
	float blurScale = 1.f;

	bool enabled = false;

public:
	DECL_SIBLING_ACCESS();

    static void registerMsgs();
    void debugInMenu();
    void load(const json& j, TEntityParseContext& ctx);

	CTexture* apply(CTexture* in_texture);
	void setEnable(bool enabled);
};
