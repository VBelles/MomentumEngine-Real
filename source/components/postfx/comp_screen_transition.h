#pragma once


class CTexture;
class CRenderToTexture;
class CRenderMesh;

class TCompScreenTransition: public TCompBase {
private:
	const CRenderTechnique* tech = nullptr;
	const CRenderMesh* mesh = nullptr;
	CRenderToTexture* rt = nullptr;
	const CTexture* texture = nullptr;
	const CTexture* transitionTexture = nullptr;

	bool transitioning = false;

	float ratio = 0.f;
	float targetRatio = 0.f;
	float speed = 1.f;

public:
	DECL_SIBLING_ACCESS();

    void debugInMenu();
    void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);

	CTexture* apply(CTexture* in_texture);

	void startTransition();
	void startTransition(float startingRatio, float finalRatio);
};
