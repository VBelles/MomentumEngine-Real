#include "mcv_platform.h"
#include "gui/widgets/gui_point.h"

using namespace GUI;

void CPoint::render() {
	MAT44 sz = MAT44::CreateScale(_params._size.x, _params._size.y, 1.f);
    EngineGUI.renderTexture(sz * _absolute,
		_states[_currentState]._texture,
		_states[_currentState]._minUV,
		_states[_currentState]._maxUV,
		_states[_currentState]._color);
}

TImageParams* CPoint::getImageParams() {
	return &_states[_currentState];
}

void CPoint::setCurrentState(EState newState) {
	_currentState = newState;
}