#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_patrol.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "render/render_objects.h"

DECL_OBJ_MANAGER("ai_patrol", CAIPatrol);

void CAIPatrol::Init()
{
	// insert all states in the map
	AddState("idle", (statehandler)&CAIPatrol::IdleState);
	AddState("seekwpt", (statehandler)&CAIPatrol::SeekWptState);
	AddState("nextwpt", (statehandler)&CAIPatrol::NextWptState);
	AddState("chase", (statehandler)&CAIPatrol::ChaseState);
	AddState("idle_war", (statehandler)&CAIPatrol::IdleWar);
	AddState("orbit_left", (statehandler)&CAIPatrol::OrbitLeft);
	AddState("orbit_right", (statehandler)&CAIPatrol::OrbitRight);
	AddState("move_back", (statehandler)&CAIPatrol::MoveBack);

	// reset the state
	ChangeState("idle");
	currentWaypoint = 0;
}
void CAIPatrol::registerMsgs() {

	DECL_MSG(CAIPatrol, TMsgDamage, OnHit);
}

void CAIPatrol::OnHit(const TMsgDamage& msg) {
	int damage = msg.damage;
	health -= damage;
	if (health < 0) {
		CHandle(this).getOwner().destroy();
	}
}

void CAIPatrol::debugInMenu() {
	IAIController::debugInMenu();
	if (ImGui::TreeNode("Waypoints")) {
		for (auto& v : _waypoints) {
			ImGui::PushID(&v);
			ImGui::DragFloat3("Point", &v.x, 0.1f, -20.f, 20.f);
			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	for (size_t i = 0; i < _waypoints.size(); ++i)
		renderLine(_waypoints[i], _waypoints[(i + 1) % _waypoints.size()], VEC4(0, 1, 0, 1));
}

void CAIPatrol::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);

	Init();

	auto& j_waypoints = j["waypoints"];
	for (auto it = j_waypoints.begin(); it != j_waypoints.end(); ++it) {
		VEC3 p = loadVEC3(it.value());
		addWaypoint(p);
	}

}


void CAIPatrol::IdleState(float delta)
{
	CEntity *player = (CEntity *)getEntityByName(PLAYER_NAME);
	TCompTransform *mypos = getMyTransform();
	TCompTransform *ppos = player->get<TCompTransform>();
	bool in_fov = mypos->isInFov(ppos->getPosition(), deg2rad(60));
	if (!in_fov) ChangeState("seekwpt");
}


void CAIPatrol::SeekWptState(float delta)
{
	TCompTransform *mypos = getMyTransform();
	float y, r, p;
	mypos->getYawPitchRoll(&y, &p, &r);
	if (mypos->isInLeft(getWaypoint()))
	{
		y += 0.001f;
	}
	else
	{
		y -= 0.001f;
	}
	mypos->setYawPitchRoll(y, p, r);
	VEC3 vp = mypos->getPosition();
	VEC3 vfwd = mypos->getFront();
	vfwd.Normalize();
	vp = vp + 0.001f*vfwd;
	mypos->setPosition(vp);
	// next wpt
	if (VEC3::Distance(getWaypoint(), vp) < 1) ChangeState("nextwpt");

	// chase
	CEntity *player = (CEntity *)getEntityByName(PLAYER_NAME);
	TCompTransform *ppos = player->get<TCompTransform>();
	bool in_fov = mypos->isInFov(ppos->getPosition(), deg2rad(60));
	if (in_fov) ChangeState("chase");
}

void CAIPatrol::NextWptState(float delta)
{
	currentWaypoint = (currentWaypoint + 1) % _waypoints.size();
	ChangeState("seekwpt");
}

void CAIPatrol::ChaseState(float delta)
{
	TCompTransform *myTransform = getMyTransform();
	CEntity *player = (CEntity *)getEntityByName(PLAYER_NAME);
	TCompTransform *playerTransform = player->get<TCompTransform>();
	float y, r, p;
	myTransform->getYawPitchRoll(&y, &p, &r);
	if (myTransform->isInLeft(playerTransform->getPosition()))
	{
		y += 0.001f;
	}
	else
	{
		y -= 0.001f;
	}
	myTransform->setYawPitchRoll(y, p, r);
	VEC3 myPosition = myTransform->getPosition();
	VEC3 myFront = myTransform->getFront();
	myFront.Normalize();
	myPosition = myPosition + 0.001f*myFront;
	myTransform->setPosition(myPosition);

	bool in_fov = myTransform->isInFov(playerTransform->getPosition(), deg2rad(60));
	if (!in_fov) {
		ChangeState("seekwpt");
	}
	else if (VEC3::Distance(myPosition, playerTransform->getPosition()) < 3) {
		ChangeState("idle_war");
	}

}

void CAIPatrol::IdleWar(float delta) {
	TCompTransform *myTransform = getMyTransform();
	CEntity *player = (CEntity *)getEntityByName(PLAYER_NAME);
	TCompTransform *playerTransform = player->get<TCompTransform>();
	VEC3 myPosition = myTransform->getPosition();
	bool in_fov = myTransform->isInFov(playerTransform->getPosition(), deg2rad(60));


	if (!in_fov) {
		ChangeState("seekwpt");
	}
	else {
		float y, r, p;
		myTransform->getYawPitchRoll(&y, &p, &r);
		if (myTransform->isInLeft(playerTransform->getPosition()))
		{
			y += 0.001f;
		}
		else
		{
			y -= 0.001f;
		}
		myTransform->setYawPitchRoll(y, p, r);
		if (VEC3::Distance(myPosition, playerTransform->getPosition()) < 2) {
			ChangeState("move_back");
		}
		else {
			int rnd = rand() % 10000;
			if (rnd == 0) {
				thisOrbitAngle = maxOrbitAngle + rand() % 30;
				ChangeState("orbit_left");
			}
			if (rnd == 1) {
				thisOrbitAngle = maxOrbitAngle + rand() % 30;
				ChangeState("orbit_right");
			}
		}
	}

}

void CAIPatrol::OrbitLeft(float delta) {
	Orbit(-orbitAngularVelocity * delta);
}

void CAIPatrol::OrbitRight(float delta) {
	Orbit(orbitAngularVelocity * delta);
}

void CAIPatrol::Orbit(float increment) {
	TCompTransform *myTransform = getMyTransform();
	CEntity *player = (CEntity *)getEntityByName(PLAYER_NAME);
	TCompTransform *playerTransform = player->get<TCompTransform>();
	float y, r, p;
	myTransform->getYawPitchRoll(&y, &p, &r);
	VEC3 myPosition = myTransform->getPosition();
	VEC3 myFront = myTransform->getFront();

	float distance = VEC3::Distance(myPosition, playerTransform->getPosition());
	myTransform->setPosition(playerTransform->getPosition());
	myTransform->setYawPitchRoll(y + increment, p, r);
	myTransform->setPosition(myTransform->getPosition() - myTransform->getFront() * distance);

	degreesOrbited += abs(increment);

	bool in_fov = myTransform->isInFov(playerTransform->getPosition(), deg2rad(60));
	if (!in_fov || distance > 4.5f) {
		degreesOrbited = 0;
		ChangeState("seekwpt");
	}
	else {
		if (distance < 2) {
			degreesOrbited = 0;
			ChangeState("move_back");
		}
		else if (rad2deg(degreesOrbited) > thisOrbitAngle) {
			degreesOrbited = 0;
			ChangeState("idle_war");
		}
	}
}

void CAIPatrol::MoveBack(float delta) {
	TCompTransform *myTransform = getMyTransform();
	CEntity *player = (CEntity *)getEntityByName(PLAYER_NAME);
	TCompTransform *playerTransform = player->get<TCompTransform>();
	float y, r, p;
	myTransform->getYawPitchRoll(&y, &p, &r);
	if (myTransform->isInLeft(playerTransform->getPosition()))
	{
		y += 0.001f;
	}
	else
	{
		y -= 0.001f;
	}
	myTransform->setYawPitchRoll(y, p, r);
	VEC3 myPosition = myTransform->getPosition();
	VEC3 myFront = myTransform->getFront();
	myFront.Normalize();
	myPosition = myPosition - 0.001f * myFront;
	myTransform->setPosition(myPosition);

	bool in_fov = myTransform->isInFov(playerTransform->getPosition(), deg2rad(60));
	if (!in_fov) {
		ChangeState("seekwpt");
	}
	else if (VEC3::Distance(myPosition, playerTransform->getPosition()) > 3) {
		ChangeState("idle_war");
	}
}

