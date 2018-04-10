#include "mcv_platform.h"
#include "module_test_cameras.h"
#include "render/render_objects.h"
#include "render/texture/material.h"
#include "entity/entity_parser.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "components/controllers/comp_curve.h"

extern CCamera camera;
extern void registerMesh(CRenderMesh* new_mesh, const char* name);

struct TVtxPosClr {
    VEC3 pos;
    VEC4 color;
    TVtxPosClr(VEC3 new_pos, VEC4 new_color) : pos(new_pos), color(new_color) {}
};

//// ---------------------------------------------------
//CRenderMesh* createCurveMesh(const CCurve& curve, int nsteps) {
//    CRenderMesh* mesh = new CRenderMesh;
//
//    std::vector<TVtxPosClr> vtxs;
//    VEC4 clr(1.f, 1.f, 1.f, 1.0f);
//    for (int i = 0; i < nsteps; ++i) {
//        vtxs.emplace_back(curve.evaluateAsCatmull((float)i / (float)nsteps), clr);
//        vtxs.emplace_back(curve.evaluateAsCatmull((float)(i + 1) / (float)nsteps), clr);
//    }
//
//    if (!mesh->create(vtxs.data(), vtxs.size() * sizeof(TVtxPosClr), "PosClr", CRenderMesh::LINE_LIST))
//        return nullptr;
//    return mesh;
//}

bool CModuleTestCameras::start() {
    CHandle h_playerCamera = getEntityByName(PLAYER_CAMERA);
    Engine.getCameras().setDefaultCamera(h_playerCamera);
    CHandle h_camera = getEntityByName(GAME_CAMERA);
    Engine.getCameras().setOutputCamera(h_camera);

    //_curve.addKnot(VEC3(8, 3, 5));
    //_curve.addKnot(VEC3(10, 3, 5));
    //registerMesh(createCurveMesh(_curve, 100), "curve.mesh");

    /*const CCurve* curve = Resources.get("data/curves/test_curve.curve")->as<CCurve>();
    registerMesh(createCurveMesh(*curve, 100), "curve.mesh");*/

    return true;
}

void CModuleTestCameras::update(float delta) {
    if (EngineInput['1'].getsPressed()) {
        CHandle h_camera = getEntityByName(PLAYER_CAMERA);
        Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY);
    }
    if (EngineInput['2'].getsPressed()) {
        CHandle h_camera = getEntityByName("fixed_camera");
        Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY);
    }
    if (EngineInput['3'].getsPressed()) {
        CHandle h_camera = getEntityByName("test_camera_curve");
        Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY);
    }

    CEntity* e_teapot = getEntityByName(GAME_CAMERA);
    if (e_teapot) {
        static float time = 0.f;
        time += delta;
        VEC3 pos(10.f * sinf(time), 0.f, 2.0f * cosf(time));
        TCompTransform* c_transform = e_teapot->get<TCompTransform>();
        c_transform->setPosition(pos);

        CEntity* e_curve = getEntityByName("test_camera_curve");
        TCompCurve* c_curve = e_curve->get<TCompCurve>();
        float ratio = (pos.x + 10.f) / 20.f;
        c_curve->setRatio(ratio);
    }
}

void CModuleTestCameras::render() {
    // Find the entity with name 'game_camera'
    CHandle h_e_camera = getEntityByName(GAME_CAMERA);
    if (h_e_camera.isValid()) {
        CEntity* e_camera = h_e_camera;
        TCompCamera* c_camera = e_camera->get<TCompCamera>();
        assert(c_camera);
        activateCamera(*c_camera, Render.width, Render.height);
    }
    else {
        activateCamera(camera, Render.width, Render.height);
    }
}
