#include "mcv_platform.h"
#include "IActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "components/comp_collider.h"
#include "components/comp_hitboxes.h"
#include "components/postfx/comp_render_blur_radial.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"
#include "components/player/comp_power_gauge.h"


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
	if (msg.info.invulnerabilityTime > 0) {
		getPlayerModel()->makeInvulnerable(msg.info.invulnerabilityTime);
	}
	//Lo que venga luego ya lo procesa el estado en concreto
}


void IActionState::setMovementInput(VEC2 input) {
	movementInput = input;
}

void IActionState::rotatePlayerTowards(float delta, VEC3 targetPos, float rotationSpeed) {
	float rotationIncrement = rotationSpeed * delta;
	float deltaYaw = getPlayerTransform()->getDeltaYawToAimTo(targetPos);
	float y, p, r;
	getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	if (abs(deltaYaw) >= rotationIncrement) {
		y = (deltaYaw > 0) ? (y + rotationIncrement) : (y - rotationIncrement);
	}
	else {
		y += deltaYaw;
	}
	getPlayerTransform()->setYawPitchRoll(y, p, r);
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
	for (HitState& hit : moveState.botHits) {
		if (hit.dotUp >= getPlayerModel()->getController()->getSlopeLimit()) {
			return true;
		}
	}
	return false;
}


//Component getters
TCompPlayerModel* IActionState::getPlayerModel() { return stateManager->getPlayerModel(); }
TCompTransform* IActionState::getPlayerTransform() { return stateManager->getTransform(); }
TCompCollider* IActionState::getCollider() { return stateManager->getCollider(); }
TCompRender* IActionState::getRender() { return stateManager->getRender(); }
TCompHitboxes* IActionState::getHitboxes() { return stateManager->getHitboxes(); }
TCompCamera* IActionState::getCamera() { return stateManager->getCamera(); }
TCompRenderBlurRadial* IActionState::getBlurRadial() { return getCamera()->get<TCompRenderBlurRadial>(); }
TCompSkeleton* IActionState::getSkeleton() { return stateManager->getSkeleton(); }
TCompPowerGauge* IActionState::getPowerGauge() { return stateManager->getPowerGauge(); }
