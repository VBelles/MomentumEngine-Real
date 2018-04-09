#include "mcv_platform.h"
#include "comp_follow_curve.h"
#include "components/comp_transform.h"
#include "geometry/curve.h"

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

    _curve->setType(j["curve_type"]);
    _curve->setLoop(j["loop"]);

    auto& j_knots = j["knots"];
    for (auto it = j_knots.begin(); it != j_knots.end(); ++it) {
        VEC3 p = loadVEC3(it.value());
        _curve->addKnot(p);
    }

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

    // actualizar ratio
    if (_automove) {
        _ratio += _speed * dt;
        if (_ratio >= 1.f) _ratio = 0.f;
    }

    // evaluar curva con dicho ratio
    VEC3 posToGo = _curve->evaluate(_ratio);
	dbg("posToGo: x: %f y: %f z: %f\n", posToGo.x, posToGo.y, posToGo.z);

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

	// TODO: Que vuelva por donde ha venido si loop = true, ahora mismo se teleporta al primer punto al acabar.
    // TODO: Añadir "is_dynamic": true al collider en el exportador de Max si tiene follow_curve.
}
