#include "mcv_platform.h"
#include "comp_follow_curve.h"
#include "components/comp_transform.h"
#include "geometry/curve.h"

DECL_OBJ_MANAGER("follow_curve", TCompFollowCurve);

void TCompFollowCurve::debugInMenu() {
}

void TCompFollowCurve::registerMsgs() {
    DECL_MSG(TCompFollowCurve, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompFollowCurve::load(const json& j, TEntityParseContext& ctx) {
    std::string curve_name = j["curve"];
    _curve = Resources.get(curve_name)->as<CCurve>(); // TODO: Sacar esto del componente, no del archivo .curve !!!

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
    _pos = _curve->evaluate(_ratio);
	//dbg("pos: x: %f y: %f z: %f\n", pos.x, pos.y, pos.z);

	// Move the component.
	TCompTransform* transform = get<TCompTransform>();
	if (!transform) return;
	/*VEC3 myPosition = transform->getPosition();

	VEC3 direction = _pos - myPosition;
	direction.Normalize();

	VEC3 _movement = direction * _speed * dt;*/

	TCompCollider* collider = get<TCompCollider>();
	assert(collider);
	PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)collider->actor;

	PxTransform newTransform;
	/*newTransform.p = {  myPosition.x + _movement.x,
                        myPosition.y + _movement.y,
                        myPosition.z + _movement.z };*/
    newTransform.p = { _pos.x, _pos.y, _pos.z };
    QUAT rotation = transform->getRotation();
	newTransform.q = { rotation.x, rotation.y, rotation.z, rotation.w };
	rigidDynamic->setKinematicTarget(newTransform);

	// TODO: Que vuelva por donde ha venido, ahora mismo se teleporta al primer punto al acabar.
    // TODO: Añadir "is_dynamic": true al collider en el exportador de Max si tiene follow_curve.

	// TODO: hacer otro componente que sea "plataforma" y sea el que comprueba si tiene algo (al player) encima y moverlo si hace falta.
}
