#include "mcv_platform.h"
#include "comp_follow_curve.h"
#include "components/comp_transform.h"
#include "geometry/curve.h"

DECL_OBJ_MANAGER("follow_curve", TCompFollowCurve);

void TCompFollowCurve::debugInMenu() {
}

void TCompFollowCurve::load(const json& j, TEntityParseContext& ctx) {
    std::string curve_name = j["curve"];
    _curve = Resources.get(curve_name)->as<CCurve>(); // TODO: Sacar esto del componente, no del archivo .curve !!!

	_speed = j.value<float>("speed", 0.f);
	_automove = j.value("automove", false);
}

void TCompFollowCurve::update(float dt) {
    if (!_curve) return;

    // actualizar ratio
    if (_automove) {
        _ratio += _speed * dt;
        if (_ratio >= 1.f) _ratio = 0.f;
    }

    // evaluar curva con dicho ratio
    VEC3 pos = _curve->evaluate(_ratio);
	//dbg("pos: x: %f y: %f z: %f\n", pos.x, pos.y, pos.z);

	// Move the component.
	TCompTransform* transform = get<TCompTransform>();
	if (!transform) return;
	VEC3 myPosition = transform->getPosition();

	TCompCollider* collider = get<TCompCollider>();
	assert(collider);
	PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;

	VEC3 direction = pos - myPosition;
	direction.Normalize();

	VEC3 movement = direction * _speed * dt;

	PxTransform newTransform;
	newTransform.p = { myPosition.x + movement.x, myPosition.y + movement.y, myPosition.z + movement.z };
	newTransform.q = { transform->getRotation().x, transform->getRotation().y, transform->getRotation().z, transform->getRotation().w };
	rigidDynamic->setKinematicTarget(newTransform);

	// TODO: Probar esto.

	// TODO: guardarse el movimiento en el .h.

	// TODO: hacer otro componente que sea "plataforma" y sea el que comprueba si tiene algo (al player) encima y moverlo si hace falta.
}
