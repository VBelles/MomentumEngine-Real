#include "mcv_platform.h"
#include "IActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/postfx/comp_render_blur_radial.h"
#include "components/player/states/StateManager.h"
#include "components/player/comp_power_gauge.h"
#include "components/controllers/comp_camera_player.h"
#include "modules/system_modules/slash/comp_slash.h"
#include "modules/system_modules/particles/comp_particles.h"
#include "components/platforms/comp_platform_simple.h"


IActionState::IActionState(StateManager* stateManager, State state, ConcurrentState concurrentState) :
	stateManager(stateManager),
	state(state),
	concurrentState(concurrentState) {
	velocityVector = getPlayerModel()->getVelocityVector();
	accelerationVector = getPlayerModel()->getAccelerationVector();

}

IActionState::IActionState(StateManager* stateManager, State state) :
	IActionState::IActionState(stateManager, state, UndefinedConcurrentState) {}

IActionState::IActionState(StateManager* stateManager, ConcurrentState concurrentState) :
	IActionState::IActionState(stateManager, UndefinedState, concurrentState) {}

void IActionState::onStateEnter(IActionState* lastState) {
	this->lastState = lastState;
	deltaMovement = VEC3::Zero;
	movementInput = VEC2::Zero;
}
void IActionState::onStateExit(IActionState* nextState) {
	this->nextState = nextState;
}

void IActionState::onDead() {
	stateManager->changeConcurrentState(Free);
	stateManager->changeState(Death);
}

void IActionState::onDamage(const TMsgAttackHit& msg) {
	getPlayerModel()->damage(msg.info.damage);
	float damageVisionTime = 0;
	if (msg.info.invulnerabilityTime > 0) {
		damageVisionTime = msg.info.invulnerabilityTime;
		getPlayerModel()->makeInvulnerable(msg.info.invulnerabilityTime);
	}
	getPlayerModel()->startDamageVision(damageVisionTime);
	//Lo que venga luego ya lo procesa el estado en concreto
}


void IActionState::setMovementInput(VEC2 input) {
	movementInput = input;
}

bool IActionState::rotatePlayerTowards(float delta, VEC3 targetPos, float rotationSpeed) {
	float rotationIncrement = rotationSpeed * delta;
	float deltaYaw = getPlayerTransform()->getDeltaYawToAimTo(targetPos);
	float y, p, r;
	getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	if (abs(deltaYaw) > rotationIncrement) {
		y = (deltaYaw > 0) ? (y + rotationIncrement) : (y - rotationIncrement);
	}
	else {
		y += deltaYaw;
		return true;
	}
	getPlayerTransform()->setYawPitchRoll(y, p, r);
	return false;
}

float IActionState::calculateAccelerationAccordingToDirection(VEC3 baseDirection, VEC3 desiredDirection, float baseAcceleration,
	float backwardsMaxDotProduct, float sidewaysMaxDotProduct, float backwardsAirDriftFactor, float sidewaysAirDriftFactor) {
	float resultingAcceleration = baseAcceleration;
	if (baseDirection.Dot(desiredDirection) <= backwardsMaxDotProduct) {
		resultingAcceleration *= backwardsAirDriftFactor;
	}
	else if (baseDirection.Dot(desiredDirection) <= sidewaysMaxDotProduct) {
		resultingAcceleration *= sidewaysAirDriftFactor;
	}
	return resultingAcceleration;
}

VEC3 IActionState::calculateHorizontalDeltaMovement(float delta, VEC3 lastFrameDirection, VEC3 newDirection,
	float acceleration, float maxSpeed) {
	//Copiamos los ejes x/z del vector de velocidad en un VEC2, para trabajar sin tocar el eje y
	VEC2 horizontalVelocity = { velocityVector->x , velocityVector->z };
	float currentSpeed = horizontalVelocity.Length();
	lastFrameDirection.Normalize();
	newDirection.Normalize();
	//Calculamos la distancia recorrida desde el �ltimo frame 
	VEC3 resultingDeltaMovement = lastFrameDirection * currentSpeed * delta + 0.5f * newDirection * acceleration * delta * delta;
	//Si ha recorrido m�s distancia que lo m�ximo permitido por la velocidad m�xima, lo clampeamos a (maxSpeed * delta)
	if (resultingDeltaMovement.Length() > maxSpeed * delta) {
		resultingDeltaMovement.Normalize();
		resultingDeltaMovement *= maxSpeed * delta;
	}
	return resultingDeltaMovement;
}

void IActionState::transferVelocityToDirectionAndAccelerate(float delta, bool wantToTransfer, VEC3 directionOfAcceleration, float acceleration) {
	//Copiamos los ejes x/z del vector de velocidad en un VEC2, para trabajar sin tocar el eje y
	VEC2 horizontalVelocity = { velocityVector->x , velocityVector->z };
	float currentSpeed = horizontalVelocity.Length();
	if (wantToTransfer) {
		//Pasamos toda la velocidad que llev�bamos a la nueva direcci�n
		directionOfAcceleration.Normalize();
		horizontalVelocity.x = directionOfAcceleration.x * currentSpeed;
		horizontalVelocity.y = directionOfAcceleration.z * currentSpeed;
	}

	//Actualizamos el vector de velocidad con la aceleraci�n
	velocityVector->x = horizontalVelocity.x + directionOfAcceleration.x * acceleration * delta;
	velocityVector->z = horizontalVelocity.y + directionOfAcceleration.z * acceleration * delta;
}

void IActionState::clampHorizontalVelocity(float maxHorizontalSpeed) {
	VEC2 horizontalVelocity;
	horizontalVelocity.x = velocityVector->x;
	horizontalVelocity.y = velocityVector->z;
	if (horizontalVelocity.Length() > maxHorizontalSpeed) {
		horizontalVelocity.Normalize();
		horizontalVelocity *= maxHorizontalSpeed;
		velocityVector->x = horizontalVelocity.x;
		velocityVector->z = horizontalVelocity.y;
	}
}

bool IActionState::isWalkable(MoveState& moveState) {
	if (moveState.botHits.empty()) {
		return true;
	}

	updateSlopeAndStep(moveState);

	float slopeLimit = getPlayerModel()->getController()->getSlopeLimit();

	for (HitState& hit : moveState.botHits) {
		if (hit.dotUp >= slopeLimit) {
			return true;
		}
	}
	return false;
}

void IActionState::updateSlopeAndStep(MoveState& moveState) {
	TCompCollider* collider = getCollider();
	if (moveState.standingShape) {
		CEntity* entity = PhysxUtils::getEntity(moveState.standingShape->getActor());
		CHandle platformHandle = entity->get<TCompPlatformSimple>();
		if (platformHandle.isValid()) {
			TCompPlatformSimple* platform = platformHandle;
			if (platform->isRolling()) {
				collider->controller->setSlopeLimit(rollingSlopeLimit);
				collider->controller->setStepOffset(0);
				return;
			}
			else {
				collider->controller->setSlopeLimit(platformSlopeLimit);
				collider->controller->setStepOffset(collider->config.step);
				return;
			}
		}
	}

	collider->controller->setSlopeLimit(cosf(deg2rad(collider->config.slope)));
	collider->controller->setStepOffset(collider->config.step);
}

bool IActionState::isStandingOnPlatform(MoveState& moveState) {
	if (moveState.standingShape) {
		CEntity* entity = PhysxUtils::getEntity(moveState.standingShape->getActor());
		CHandle platformHandle = entity->get<TCompPlatformSimple>();
		if (platformHandle.isValid()) {
			return true;
		}
	}
	return false;
}

void IActionState::slash(std::string slash, VEC3 offset, float yaw, float pitch, float roll) {
	float y = 0, p = 0;
	getYawPitchFromVector(getPlayerTransform()->getFront(), &y, &p);
	Particles::LaunchConfig config;
	config.targetEntity = getPlayerEntity();
	config.rotationOffset =
		QUAT::CreateFromAxisAngle(VEC3(1, 0, 0), pitch + deg2rad(90)) *
		QUAT::CreateFromAxisAngle(VEC3(0, 1, 0), yaw) *
		QUAT::CreateFromAxisAngle(VEC3(0, 0, 1), roll) *
		// QUAT::CreateFromYawPitchRoll(deg2rad(yaw), deg2rad(pitch), deg2rad(roll)) *
		QUAT::CreateFromYawPitchRoll(y, p, 0);
	config.offset = offset;
	EngineParticles.launchSystem(slash, config);
}

//Component getters
TCompPlayerModel* IActionState::getPlayerModel() {
	return stateManager->getPlayerModel();
}

CEntity* IActionState::getPlayerEntity() {
	return getPlayerModel()->getPlayerEntity();
}

TCompTransform* IActionState::getPlayerTransform() {
	return getPlayerModel()->getTransform();
}

TCompCollider* IActionState::getCollider() {
	return getPlayerModel()->getCollider();
}

TCompRender* IActionState::getRender() {
	return getPlayerModel()->getRender();
}

TCompHitboxes* IActionState::getHitboxes() {
	return getPlayerModel()->getHitboxes();
}

TCompCamera* IActionState::getCamera() {
	return getPlayerModel()->getCameraRender();
}

TCompRenderBlurRadial* IActionState::getBlurRadial() {
	return getCamera()->get<TCompRenderBlurRadial>();
}

TCompSkeleton* IActionState::getSkeleton() {
	return getPlayerModel()->getSkeleton();
}

TCompPowerGauge* IActionState::getPowerGauge() {
	return getPlayerModel()->getPowerGauge();
}

TCompCollectableManager* IActionState::getCollectableManager() {
	return getPlayerModel()->getCollectableManager();
}

TCompCameraPlayer* IActionState::getCameraPlayer() {
	return getPlayerModel()->getCameraPlayer();
}

TCompSound * IActionState::getSound() {
	return getPlayerModel()->getSound();
}

TCompSlash * IActionState::getTrailSlash(SlashType type) {
	return stateManager->getTrailSlash(type);
}

TCompParticles * IActionState::getParticles() {
	return getPlayerModel()->getParticles();
}
