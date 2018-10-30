#include "mcv_platform.h"
#include "module_gui.h"
#include "render/render_objects.h"
#include "gui/gui_parser.h"
#include <limits>

using namespace GUI;

CModuleGUI::CModuleGUI(const std::string& name)
	: IModule(name) {
}

bool CModuleGUI::start() {
	_orthoCamera.setPerspective(-1.f, 1.f, resolution.x, resolution.y);

	_technique = Resources.get("gui.tech")->as<CRenderTechnique>();
	_technique_clamp = Resources.get("gui_clamp.tech")->as<CRenderTechnique>();
	_technique_font = Resources.get("gui_font.tech")->as<CRenderTechnique>();
	_quadMesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();
	_fontTexture = Resources.get("data/textures/gui/font_sheet.dds")->as<CTexture>();
	_nullMaskTexture = Resources.get("data/textures/white.dds")->as<CTexture>();

	json jFonts = loadJson(fontsFile);
	for (auto it = jFonts.begin(); it != jFonts.end(); ++it) {
		CFont font;
		font.load(it.value());
		fonts[font.getSize()] = font;
	}

	dialogController = new CDialogController();

	return true;
}

bool CModuleGUI::stop() {
	clearWidgets();
	_variables.clear();
	for (auto controller : _controllers) {
		safeDelete(controller);
	}
	_controllers.clear();
	return true;
}

void CModuleGUI::update(float delta) {
	if (dialogActive) dialogController->update(delta);
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

	if (widget && recursive) {
		VWidgets children = widget->getChildren(recursive);
		for (CWidget* child : children) {
			auto it = std::find(_activeWidgets.begin(), _activeWidgets.end(), child);
			if (it != _activeWidgets.end()) {
				_activeWidgets.erase(it);
			}

			it = std::find(_registeredWidgets.begin(), _registeredWidgets.end(), child);
			if (it != _registeredWidgets.end()) {
				safeDelete(*it);
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
		safeDelete(*it);
		_registeredWidgets.erase(it);
	}
}

void CModuleGUI::clearWidgets() {
	for (auto widget : _registeredWidgets) {
		safeDelete(widget);
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

void CModuleGUI::renderTexture(const MAT44& world, const CTexture* texture, const VEC2& minUV, const VEC2& maxUV,
	const VEC4& color, const CTexture* mask, const VEC2& maskMinUV, const VEC2& maskMaxUV, const std::string& tech) {
	assert(_technique && _quadMesh);

	cb_object.obj_world = world;
	cb_object.obj_color = VEC4(1, 1, 1, 1);
	cb_object.updateGPU();

	cb_gui.minUV = minUV;
	cb_gui.maxUV = maxUV;
	cb_gui.tint_color = color;
	cb_gui.charSize = maskMinUV;
	cb_gui.dummy_GUI = maskMaxUV;
	cb_gui.updateGPU();

	if (tech == "clamp") {
		_technique_clamp->activate();
	}
	else {
		_technique->activate();
	}

	if (texture) texture->activate(TS_ALBEDO);
	if (mask) {
		mask->activate(TS_NORMAL);
	}
	else {
		_nullMaskTexture->activate(TS_NORMAL);
	}

	_quadMesh->activateAndRender();
}

void CModuleGUI::renderText(const MAT44& world, const CTexture* texture, const VEC2& minUV, const VEC2& maxUV, const VEC4& color, const VEC2& charSize) {
	assert(_technique_font && _quadMesh);

	cb_object.obj_world = world;
	cb_object.obj_color = VEC4(1, 1, 1, 1);
	cb_object.updateGPU();

	cb_gui.minUV = minUV;
	cb_gui.maxUV = maxUV;
	cb_gui.tint_color = color;
	cb_gui.charSize = charSize;
	cb_gui.updateGPU();

	_technique_font->activate();
	if (texture) texture->activate(TS_ALBEDO);

	_quadMesh->activateAndRender();
}

CFont& CModuleGUI::getFont(int size) {
	assert(fonts.size() >= 1);
	auto it = fonts.find(size);
	if (it != fonts.end()) {
		return it->second;
	}
	else {
		int index = std::numeric_limits<int>::max();

		for (auto it : fonts) {
			if (abs(it.first - size) < abs(index - size)) {
				index = it.first;
			}
		}

		return fonts[index];
	}
}

void CModuleGUI::showDialog(const std::string& text, const int& fontSize, bool cancelable) {
	dialogController->showDialog(text, fontSize, cancelable);
}

void CModuleGUI::hideDialog() {
	dialogController->hideDialog();
}

void CModuleGUI::setDialogActive(bool active) {
	dialogActive = active;
	dialogController->setVisible(active);
}

bool CModuleGUI::isDialogActive() {
	return dialogController->isActive();
}

void CModuleGUI::setCancelableWithButton(bool cancelable) {
	dialogController->setCancelableWithButton(cancelable);
}

VEC2 CModuleGUI::getResolution() {
	return resolution;
}