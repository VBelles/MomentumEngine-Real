#pragma once

class TCompPlayerModel;
class TCompTransform;
class TCompCollider;

class ScriptingPlayer {

private:
	CHandle playerModelHandle;
	CHandle transformHandle;
	CHandle colliderHandle;
	TCompPlayerModel* getPlayerModel();
	TCompTransform* getTransform();
	TCompCollider* getCollider();

public:
	ScriptingPlayer();
	~ScriptingPlayer();
	float getHp();
	void teleport(float x, float y, float z);
};

