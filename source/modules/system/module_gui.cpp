#include "mcv_platform.h"
#include "module_gui.h"
#include "render/render_objects.h"
#include "gui/gui_parser.h"
#include "gui/controllers/gui_main_menu_controller.h"

using namespace GUI;

CModuleGUI::CModuleGUI(const std::string& name)
	: IModule(name) {
}

bool CModuleGUI::start() {
	const float width = 1920;
	const float height = 1080;
	_orthoCamera.setPerspective(-1.f, 1.f, width, height);

	_technique = Resources.get("gui.tech")->as<CRenderTechnique>();
	_quadMesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();
	_fontTexture = Resources.get("data/textures/gui/font.dds")->as<CTexture>();

	return true;
}

bool CModuleGUI::stop() {
	clearWidgets();
	_variables.clear();
	for (auto controller : _controllers) {
		SAFE_DELETE(controller);
	}
	_controllers.clear();
	return true;
}

void CModuleGUI::update(float delta) {
	for (auto& wdgt : _activeWidgets) {
		wdgt->updateAll(delta);
	}
	for (auto& controller : _controllers) {
		controller->update(delta);
	}
}

void CModuleGUI::renderGUI() {
	for (auto& wdgt : _activeWidgets) {
		wdgt->renderAll();
	}
}

void CModuleGUI::registerWidget(CWidget* wdgt) {
	_registeredWidgets.push_back(wdgt);
}

void CModuleGUI::unregisterWidget(std::string name, bool recursive) {
	CWidget* widget = getWidget(name, recursive);

	if (recursive) {
		VWidgets children = widget->getChildren(recursive);
		for (CWidget* child : children) {
			auto it = std::find(_activeWidgets.begin(), _activeWidgets.end(), child);
			if (it != _activeWidgets.end()) {
				_activeWidgets.erase(it);
			}

			it = std::find(_registeredWidgets.begin(), _registeredWidgets.end(), child);
			if (it != _registeredWidgets.end()) {
				SAFE_DELETE(*it);
				_registeredWidgets.erase(it);
			}
		}
	}

	auto it = std::find(_activeWidgets.begin(), _activeWidgets.end(), widget);
	if (it != _activeWidgets.end()) {
		_activeWidgets.erase(it);
	}

	it = std::find(_registeredWidgets.begin(), _registeredWidgets.end(), widget);
	if (it != _registeredWidgets.end()) {
		SAFE_DELETE(*it);
		_registeredWidgets.erase(it);
	}
}

void CModuleGUI::clearWidgets() {
	for (auto widget : _registeredWidgets) {
		SAFE_DELETE(widget);
	}
	_registeredWidgets.clear();
	_activeWidgets.clear();
}

CWidget* CModuleGUI::getWidget(const std::string& name, bool recursive) const {
	for (auto& rwdgt : _registeredWidgets) {
		if (rwdgt->getName() == name) {
			return rwdgt;
		}
	}

	if (recursive) {
		for (auto& rwdgt : _registeredWidgets) {
			CWidget* wdgt = rwdgt->getChild(name, true);
			if (wdgt) {
				return wdgt;
			}
		}
	}

	return nullptr;
}

void CModuleGUI::activateWidget(const std::string& name) {
	CWidget* wdgt = getWidget(name);
	if (wdgt) {
		_activeWidgets.push_back(wdgt);
	}
}

void CModuleGUI::deactivateWidget(const std::string& name) {
	CWidget* wdgt = getWidget(name);
	auto it = std::find(_activeWidgets.begin(), _activeWidgets.end(), wdgt);
	if (it != _activeWidgets.end()) {
		_activeWidgets.erase(it);
	}
}

void CModuleGUI::registerController(GUI::CController* controller) {
	auto it = std::find(_controllers.begin(), _controllers.end(), controller);
	if (it == _controllers.end()) {
		_controllers.push_back(controller);
	}
}

void CModuleGUI::unregisterController(GUI::CController* controller) {
	auto it = std::find(_controllers.begin(), _controllers.end(), controller);
	if (it != _controllers.end()) {
		_controllers.erase(it);
	}
}

CCameraGui& CModuleGUI::getCamera() {
	return _orthoCamera;
}

MVariants& CModuleGUI::getVariables() {
	return _variables;
}

void CModuleGUI::renderTexture(const MAT44& world, const CTexture* texture, const VEC2& minUV, const VEC2& maxUV, const VEC4& color) {
	assert(_technique && _quadMesh);

	cb_object.obj_world = world;
	cb_object.obj_color = VEC4(1, 1, 1, 1);
	cb_object.updateGPU();

	cb_gui.minUV = minUV;
	cb_gui.maxUV = maxUV;
	cb_gui.tint_color = color;
	cb_gui.updateGPU();

	_technique->activate();
	if (texture)
		texture->activate(TS_ALBEDO);

	_quadMesh->activateAndRender();
}

void CModuleGUI::renderText(const MAT44& world, const std::string& text, const VEC4& color) {
	assert(_fontTexture);

	int cellsPerRow = 8;
	float cellSize = 1.f / 8.f;
	char firstCharacter = ' ';
	for (size_t i = 0; i < text.size(); ++i) {
		char c = text[i];

		int cell = c - firstCharacter;
		int row = cell / cellsPerRow;
		int col = cell % cellsPerRow;

		VEC2 minUV = VEC2(col * cellSize, row * cellSize);
		VEC2 maxUV = minUV + VEC2(1, 1) * cellSize;
		VEC2 gap = i * VEC2(1, 0);
		MAT44 w = MAT44::CreateTranslation(gap.x, gap.y, 0.f) * world;

		renderTexture(w, _fontTexture, minUV, maxUV, color);
	}
}
