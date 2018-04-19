#include "mcv_platform.h"
#include "comp_follow_curve.h"
#include "components/comp_transform.h"
#include "geometry/curve.h"
#include "components/comp_collider.h"

#include <algorithm>

DECL_OBJ_MANAGER("follow_curve", TCompFollowCurve);

void TCompFollowCurve::debugInMenu() {
    ImGui::DragFloat("speed", &_speed, 0.01f, 0.f, 20.f);
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
    _curve->setType(j["curve_type"]);
    _curve->setLoop(j["loop"]);

    auto& j_knots = j["knots"];
    for (auto it = j_knots.begin(); it != j_knots.end(); ++it) {
        VEC3 p = loadVEC3(it.value());
        _curve->addKnot(p);
    }

	_curve->calculateRadius();

	_speed = j.value<float>("speed", 0.f);
	_automove = j.value("automove", false);
}

void TCompFollowCurve::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
    TCompCollider* collider = get<TCompCollider>();
    assert(collider);
    PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;
    rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}

void TCompFollowCurve::update(float dt) {
    if (!_curve) return;

    // Actualizar ratio
    if (_automove) {
        if (!_moveBackwards) {
            _ratio += _speed * dt;
            if (_ratio >= 1.f) { // Reaches the end of the spline.
                if (_curve->isLooping()) _moveBackwards = true;
                else _automove = false; // If doesn't loop, stop moving.
            }
        }
        else {
            _ratio -= _speed * dt;
            if (_ratio <= 0.f) _moveBackwards = false;
        }
    }

    // Evaluar curva con dicho ratio
    VEC3 posToGo = _curve->evaluate(_ratio);
	//dbg("posToGo: x: %f y: %f z: %f\n", posToGo.x, posToGo.y, posToGo.z);

    TCompTransform* transform = get<TCompTransform>();
	if (!transform) return;
    VEC3 myPosition = transform->getPosition();

    _movement = posToGo - myPosition;

	// Move the component.
	TCompCollider* collider = get<TCompCollider>();
	assert(collider);
	PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)collider->actor;

	PxTransform newTransform;
    newTransform.p = { posToGo.x, posToGo.y, posToGo.z };
	transform->setPosition({ posToGo.x, posToGo.y, posToGo.z }); // Avoid position shift.
    QUAT rotation = transform->getRotation();
	newTransform.q = { rotation.x, rotation.y, rotation.z, rotation.w };
	rigidDynamic->setKinematicTarget(newTransform);
}
