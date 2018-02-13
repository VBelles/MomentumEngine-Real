#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_combat_controller.h"
#include "comp_transform.h"

DECL_OBJ_MANAGER("combat_controller", TCompCombatController);

void TCompCombatController::debugInMenu()
{
    ImGui::Text("State %s", state.c_str());
    ImGui::DragFloat("FoV angle", &fovAngle, 0.1f, 0.f, 359.f);
    ImGui::DragFloat("Fov distance", &fovDist, 0.1f, 0.f, 100.f);
    ImGui::DragFloat("Speed", &speed, 0.1f, 0.f, 20.f);
    ImGui::DragFloat("Combat distance", &combatDist, 0.1f, 0.f, 100.f);
    ImGui::DragInt("Hit points", &hitPoints, 1, 0, 10);
    ImGui::Text("Yaw %f", rad2deg(currYaw));
    if (h_target.isValid())
    {
        if (ImGui::TreeNode("Target"))
        {
            h_target.debugInMenu();
            ImGui::TreePop();
        }
    }
}

void TCompCombatController::onHit(const TMsgDamage& msg)
{
    ChangeState("hit");
}

void TCompCombatController::registerMsgs()
{
    DECL_MSG(TCompCombatController, TMsgDamage, onHit);
}

void TCompCombatController::load(const json& j, TEntityParseContext& ctx)
{
    target_name = j.value("target", "");

    h_target = ctx.findEntityByName(target_name);

    Init();
}

void TCompCombatController::update(float dt)
{
    //if (!h_target.isValid()) return;

    //(this->*statemap[state])();
    if      (state.compare("idle")      == 0) IdleState();
    else if (state.compare("chase")     == 0) Chase();
    else if (state.compare("idleWar")   == 0) IdleWar();
    else if (state.compare("orbit")     == 0) Orbit();
    else if (state.compare("back")      == 0) Back();
    else if (state.compare("hit")       == 0) Hit();
}

void TCompCombatController::ChangeState(const std::string& newstate)
{
    // try to find a state with the suitable name
    if (statemap.find(newstate) == statemap.end())
    {
        // the state we wish to jump to does not exist. we abort
        exit(-1);
    }
    state = newstate;
}

void TCompCombatController::Init()
{
    // Insert all states in the map
    AddState("idle",    (statehandler)&TCompCombatController::IdleState);
    AddState("chase",   (statehandler)&TCompCombatController::Chase);
    AddState("idleWar", (statehandler)&TCompCombatController::IdleWar);
    AddState("orbit",   (statehandler)&TCompCombatController::Orbit);
    AddState("back",    (statehandler)&TCompCombatController::Back);
    AddState("hit",     (statehandler)&TCompCombatController::Hit);

    orbitLeft = (rand() % 2 == 0);

    // reset the state
    ChangeState("idle");
}

void TCompCombatController::AddState(const std::string& name, statehandler sh)
{
    // Try to find a state with the suitable name.
    if (statemap.find(name) != statemap.end())
    {
        // The state we wish to jump to already exists. Abort.
        exit(-1);
    }
    statemap[name] = sh;
}

void TCompCombatController::IdleState()
{
    if (!h_target.isValid()) return;

    CEntity* e_target = h_target;
    assert(e_target);

    TCompTransform *c_target = e_target->get<TCompTransform>();
    assert(c_target);

    TCompTransform *c_my_transform = get<TCompTransform>();
    assert(c_my_transform);

    VEC3 targetPos = c_target->getPosition();
    VEC3 myPos = c_my_transform->getPosition();

    if (c_my_transform->isInFov(targetPos, deg2rad(fovAngle)) &&
        VEC3::Distance(myPos, targetPos) <= fovDist)
    {
        ChangeState("chase");
    }
}

void TCompCombatController::Chase()
{
    if (!h_target.isValid()) return;

    CEntity* e_target = h_target;
    assert(e_target);

    TCompTransform *c_target = e_target->get<TCompTransform>();
    assert(c_target);

    TCompTransform *c_my_transform = get<TCompTransform>();
    assert(c_my_transform);

    VEC3 targetPos = c_target->getPosition();
    VEC3 myPos = c_my_transform->getPosition();

    // Chase the target.
    VEC3 my_new_pos = targetPos - myPos;
    my_new_pos.Normalize();
    my_new_pos = myPos + my_new_pos * speed; // Move forward.
    c_my_transform->lookAt(my_new_pos, targetPos);
        
    if (VEC3::Distance(my_new_pos, targetPos) < combatDist)
    {
        ChangeState("idleWar");
    }
}

void TCompCombatController::IdleWar()
{
    if (!h_target.isValid()) return;
    
    CEntity* e_target = h_target;
    assert(e_target);

    TCompTransform *c_target = e_target->get<TCompTransform>();
    assert(c_target);

    TCompTransform *c_my_transform = get<TCompTransform>();
    assert(c_my_transform);

    VEC3 targetPos = c_target->getPosition();
    VEC3 myPos = c_my_transform->getPosition();
    
    c_my_transform->lookAt(myPos, targetPos);

    if (VEC3::Distance(myPos, targetPos) < backDist)
    {
        ChangeState("back");
    }
    if (rand() % 1000 < 5)
    {
        ChangeState("orbit");
    }
}

void TCompCombatController::Orbit()
{
    if (!h_target.isValid()) return;
    
    CEntity* e_target = h_target;
    assert(e_target);

    TCompTransform *c_target = e_target->get<TCompTransform>();
    assert(c_target);

    TCompTransform *c_my_transform = get<TCompTransform>();
    assert(c_my_transform);

    VEC3 initialPos = c_my_transform->getPosition();
    VEC3 targetPos = c_target->getPosition();
    float distToTarget = VEC3::Distance(initialPos, targetPos);
    // Move me to target position.
    c_my_transform->setPosition(targetPos);

    // Current orientation.
    float current_pitch = 0.f;
    c_my_transform->getYawPitchRoll(&currYaw, &current_pitch);

    // Rotate.
    orbitLeft ? currYaw -= speed : currYaw += speed;
    rotatedAmount += speed;

    c_my_transform->setYawPitchRoll(currYaw, current_pitch, 0.f);
    VEC3 vecBack = c_my_transform->getFront();
    vecBack.Normalize();
    VEC3 myNewPos = c_my_transform->getPosition();
    // Go back to place.
    c_my_transform->setPosition(myNewPos - vecBack * distToTarget);

    if (distToTarget < backDist)   ChangeState("back");
    if (distToTarget > combatDist) ChangeState("chase");

    float targetRotationAmount = static_cast<float>(60 + rand() % 30);
    if (rad2deg(rotatedAmount) > targetRotationAmount)
    {
        rotatedAmount = 0.f;
        orbitLeft = (rand() % 2 == 0);
        ChangeState("idleWar");
    }
}

void TCompCombatController::Back()
{
    if (!h_target.isValid()) return;

    CEntity* e_target = h_target;
    assert(e_target);

    TCompTransform *c_target = e_target->get<TCompTransform>();
    assert(c_target);

    TCompTransform *c_my_transform = get<TCompTransform>();
    assert(c_my_transform);

    VEC3 targetPos = c_target->getPosition();
    VEC3 myPos = c_my_transform->getPosition();

    // Run from the target.
    VEC3 my_new_pos = targetPos - myPos;
    my_new_pos.Normalize();
    my_new_pos = myPos - my_new_pos * speed; // Move back.
    c_my_transform->lookAt(my_new_pos, targetPos);

    if (VEC3::Distance(my_new_pos, targetPos) > combatDist)
    {
        ChangeState("idleWar");
    }
}

void TCompCombatController::Hit()
{
    if (!h_target.isValid()) return;

    hitPoints--;
    if (hitPoints <= 0)
    {
        CHandle(this).getOwner().destroy();
    }
    ChangeState("idle"); // It should go back to previous state instead of idle.
}
