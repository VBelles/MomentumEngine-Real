#include "mcv_platform.h"
#include "module_test_axis.h"
#include "camera/camera.h"
#include "render/render_objects.h"
#include "entity/entity.h"
#include "modules/module_entities.h"
#include "resources/resources_manager.h"
#include "render/texture/texture.h"
#include "render/texture/material.h"
#include "render/shaders/render_technique.h"
#include "handle/handle.h"
#include "components/comp_name.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/entity_parser.h"
#include "render/render_manager.h"

CCamera camera;

bool CModuleTestAxis::start() {
    {
        TEntityParseContext ctx;
        parseScene("data/scenes/player.scene", ctx);
    }
    {
        TEntityParseContext ctx;
        parseScene("data/scenes/scene_basic.scene", ctx);
    }
    {
        TEntityParseContext ctx;
        parseScene("data/scenes/whitebox23.scene", ctx);
    }
    {
        TEntityParseContext ctx;
        parseScene("data/scenes/test_cameras.scene", ctx);
    }
    {
        TEntityParseContext ctx;
        parseScene("data/scenes/game_camera.scene", ctx);
    }

	if (!cb_camera.create(CB_CAMERA)) return false;
    if (!cb_object.create(CB_OBJECT)) return false;

    cb_object.activate();
    cb_camera.activate();

    return true;
}

bool CModuleTestAxis::stop() {
    cb_camera.destroy();
    cb_object.destroy();
    return true;
}

void CModuleTestAxis::update(float delta) {
}

void CModuleTestAxis::render() {
    // Render the grid
    cb_object.obj_world = MAT44::Identity;
    cb_object.obj_color = VEC4(1, 1, 1, 1);
    cb_object.updateGPU();

    auto solid = Resources.get("data/materials/solid.material")->as<CMaterial>();
    solid->activate();
}
