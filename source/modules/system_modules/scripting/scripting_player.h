#pragma once

class TCompPlayerModel;
class TCompTransform;
class TCompCollider;
class TCompPowerGauge;
class TCompPlayerController;
class TCompCameraPlayer;

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
    static TCompCameraPlayer* getPlayerCamera();

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
    static void lockPlayerCameraInput();
    static void unlockPlayerCameraInput();
	static void disablePlayerOutline();
	static void enablePlayerOutline();
	static bool isPlayerGrounded();
	static void stopPlayerVelocity();
	static void changePlayerState(std::string state);
	static void changePlayerConcurrentState(std::string state);
	static void walkTo(float x, float z, float speed);
};
