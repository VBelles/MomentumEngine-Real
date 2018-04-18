#pragma once

class TCompPlayerModel;
class TCompTransform;
class TCompCollider;
class TCompPowerGauge;

class ScriptingPlayer {

private:
	CHandle playerModelHandle;
	CHandle transformHandle;
	CHandle colliderHandle;
	CHandle powerGaugeHandle;
	TCompPlayerModel* getPlayerModel();
	TCompTransform* getTransform();
	TCompCollider* getCollider();
	TCompPowerGauge* getPowerGauge();

public:
	ScriptingPlayer();
	~ScriptingPlayer();
	float getHp();
	void teleport(float x, float y, float z);
	void move(float dX, float dY, float dZ);
	void setPower(float power);
};

