#include "mcv_platform.h"
#include "gui_map_marker.h"
#include "gui/widgets/gui_text.h"

using namespace GUI;

CMapMarker::~CMapMarker() {
	SAFE_DELETE(_button);
	SAFE_DELETE(_marker);
}


void CMapMarker::render() {
	// render button
	if (!_visible) {
		std::string text = _button->getTextParams()->_text;
		auto templates = _button->getTextParams()->_templateText.texts;

		_button->getTextParams()->_templateText.texts.clear();
		_button->getTextParams()->_text = alternText;

		_button->render();

		_button->getTextParams()->_text = text;
		_button->getTextParams()->_templateText.texts = templates;
	}
	else {
		_button->render();
	}

	//render marker
	if (_visible) _marker->render();
}

void CMapMarker::setCurrentState(CButton::EState newState) {
	_button->setCurrentState(newState);
	_marker->setCurrentState(newState);
}

void CMapMarker::computeAbsolute() {
	CWidget::computeAbsolute();
	_button->setParent(_parent);
	_marker->setParent(EngineGUI.getWidget(mapWidget, true));
	_button->computeAbsolute();
	_marker->computeAbsolute();
}

bool CMapMarker::overlaps(VEC2 pos) {
	return _button->overlaps(pos) || (_visible && _marker->overlaps(pos));
}

bool CMapMarker::isVisible() {
	return _visible;
}

void CMapMarker::setVisible(bool visible) {
	_visible = visible;
}

CButton* CMapMarker::getButton() {
	return _button;
}

CButton* CMapMarker::getMarker() {
	return _marker;
}

VEC3 CMapMarker::getPos() {
	return pos;
}