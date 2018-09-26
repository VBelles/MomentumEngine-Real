#pragma once


class  CTexture;
class CRenderToTexture;
class CRenderMesh;

class TCompRenderFog: public TCompBase {
private:
	const CRenderTechnique* tech = nullptr;
	const CRenderMesh* mesh = nullptr;
	CRenderToTexture* rt = nullptr;

	VEC4 fogColor;
	float fogDensity = 0.f;
	float fogMaxRatio = 0.f;

	bool enabled = false;

public:
	DECL_SIBLING_ACCESS();

    static void registerMsgs();
    void debugInMenu();
    void load(const json& j, TEntityParseContext& ctx);

	CTexture* apply(CTexture* in_texture);
	void setEnable(bool enabled);
};
