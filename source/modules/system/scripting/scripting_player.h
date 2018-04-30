#pragma once

class TCompPlayerModel;
class TCompTransform;
class TCompCollider;
class TCompPowerGauge;
class TCompPlayerController;

namespace SLB{
	class Manager;
}

class ScriptingPlayer {

private:
	CHandle playerModelHandle;
	CHandle playerControllerHandle;
	CHandle transformHandle;
	CHandle colliderHandle;
	CHandle powerGaugeHandle;
	TCompPlayerModel* getPlayerModel();
	TCompPlayerController* getPlayerController();
	TCompTransform* getTransform();
	TCompCollider* getCollider();
	TCompPowerGauge* getPowerGauge();

public:
	ScriptingPlayer();
	~ScriptingPlayer();
	static void bind(SLB::Manager* manager);
	float getHp();
	float setHp(float hp);
	void teleport(float x, float y, float z);
	void move(float dX, float dY, float dZ);
	void setPower(float power);
	void setRespawnPosition(float x, float y, float z);
	bool takePlayerControl();
	bool givePlayerControl();
};

