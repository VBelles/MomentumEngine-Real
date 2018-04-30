#pragma once

class TCompPlayerModel;
class TCompTransform;
class TCompCollider;
class TCompPowerGauge;
class TCompPlayerController;

namespace SLB {
	class Manager;
}

class ScriptingPlayer {

private:
	static TCompPlayerModel* getPlayerModel();
	static TCompPlayerController* getPlayerController();
	static TCompTransform* getTransform();
	static TCompCollider* getCollider();
	static TCompPowerGauge* getPowerGauge();

public:
	static void bind(SLB::Manager* manager);
	static float getPlayerHp();
	static float setPlayerHp(float hp);
	static void teleportPlayer(float x, float y, float z);
	static void movePlayer(float dX, float dY, float dZ);
	static void setPower(float power);
	static void setPlayerRespawnPosition(float x, float y, float z);
	static bool takePlayerControl();
	static bool givePlayerControl();
};

