#include "mcv_platform.h"
#include "comp_follow_curve.h"
#include "components/comp_transform.h"
#include "geometry/curve.h"
#include "components/comp_collider.h"

#include <algorithm>

DECL_OBJ_MANAGER("follow_curve", TCompFollowCurve);

void TCompFollowCurve::debugInMenu() {
    ImGui::DragFloat("speed", &speed, 0.01f, 0.f, 20.f);
}

void TCompFollowCurve::registerMsgs() {
    DECL_MSG(TCompFollowCurve, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompFollowCurve::load(const json& j, TEntityParseContext& ctx) {
    //std::string curve_name = j["curve"];
    //_curve = Resources.get(curve_name)->as<CCurve>();

	/*
	TODO: Lo fino aquí sería hacer un _curve->load(j), donde j es
		el contenido del json del componente follow_curve.
		Ahora mismo lo hacemos guarro a mano, replicando el load.
	*/
    curve->setType(j["curve_type"]);
    curve->setLoop(j["loop"]);

    auto& j_knots = j["knots"];
    for (auto it = j_knots.begin(); it != j_knots.end(); ++it) {
        VEC3 p = loadVEC3(it.value());
        curve->addKnot(p);
    }

	curve->calculateRadius();

	speed = j.value<float>("speed", 0.f);
	automove = j.value("automove", false);
}

void TCompFollowCurve::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
    transformHandle = get<TCompTransform>();
    colliderHandle = get<TCompCollider>();
        
    PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)getCollider()->actor;
    rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}

void TCompFollowCurve::update(float dt) {
    if (!curve) return;

    TCompTransform* transform = getTransform();
    VEC3 myPosition = transform->getPosition();

    VEC3 posToGo;
    if (automove) {
        if (curve->getType() == CCurve::EType::CIRCULAR) {
            posToGo = orbit(dt, curve->getCenter(), speed);
        }
        else {
            // Actualizar ratio
            if (!moveBackwards) {
                ratio += speed * dt;
                if (ratio >= 1.f) { // Reaches the end of the spline.
                    if (curve->isLooping()) moveBackwards = true;
                    else automove = false; // If doesn't loop, stop moving.
                }
            }
            else {
                ratio -= speed * dt;
                if (ratio <= 0.f) moveBackwards = false;
            }
            // Evaluar curva con dicho ratio
            posToGo = curve->evaluate(ratio);
            //dbg("posToGo: x: %f y: %f z: %f\n", posToGo.x, posToGo.y, posToGo.z);
        }
    }

    movement = posToGo - myPosition;

	// Move the component.
	PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)getCollider()->actor;

	PxTransform newTransform;
    newTransform.p = { posToGo.x, posToGo.y, posToGo.z };
	transform->setPosition({ posToGo.x, posToGo.y, posToGo.z }); // Avoid position shift.
    QUAT rotation = transform->getRotation();
	newTransform.q = { rotation.x, rotation.y, rotation.z, rotation.w };
	rigidDynamic->setKinematicTarget(newTransform);
}

VEC3 TCompFollowCurve::orbit(float dt, VEC3 targetPos, float rotationSpeed) {
    VEC3 front = getTransform()->getFront();

    float yaw, pitch;
    getYawPitchFromVector(front, &yaw, &pitch);

    yaw += speed * dt;

    VEC3 newFront = getVectorFromYawPitch(yaw, pitch);
    VEC3 newPos = targetPos - newFront * curve->getRadius();
    getTransform()->lookAt(newPos, targetPos);
    return newPos;
}
