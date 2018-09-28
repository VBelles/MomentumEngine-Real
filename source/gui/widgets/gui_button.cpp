#include "mcv_platform.h"
#include "gui_button.h"

using namespace GUI;

void CButton::render() {
	TButtonParams& btParams = _states[_currentState];

	// render image
	MAT44 sz = MAT44::CreateScale(_params._size.x, _params._size.y, 1.f);
    EngineGUI.renderTexture(sz * _absolute,
                            btParams._imageParams._texture,
                            btParams._imageParams._minUV,
                            btParams._imageParams._maxUV,
                            btParams._imageParams._color);

	// render text
	CFont font = EngineGUI.getFont(btParams._textParams._size);

	float textWidth = font.getWidth(btParams._textParams._text);
	float textHeight = font.getHeight();

	VEC2 textSpace = _params._size;
	VEC2 offset;
	if (btParams._textParams._hAlign == TTextParams::Center)
		offset.x = textSpace.x * 0.5f - textWidth * 0.5f;
	else if (btParams._textParams._hAlign == TTextParams::Right)
		offset.x = textSpace.x - textWidth;
	if (btParams._textParams._vAlign == TTextParams::Center)
		offset.y = _params._size.y * 0.5f - textHeight * 0.5f;
	else if (btParams._textParams._vAlign == TTextParams::Bottom)
		offset.y = _params._size.y - textHeight;

	MAT44 tr = MAT44::CreateTranslation(offset.x, offset.y, 0.f);
	MAT44 w = MAT44::CreateScale(font.getSize()) * tr * _absolute;
	font.renderText(w, btParams._textParams._text, btParams._textParams._color);
}

TButtonParams* CButton::getButtonParams(EState state) {
	return &_states[state];
}

TImageParams* CButton::getImageParams() {
	return &_states[_currentState]._imageParams;
}

TTextParams* CButton::getTextParams() {
	return &_states[_currentState]._textParams;
}

void CButton::setCurrentState(EState newState) {
	_currentState = newState;
	computeAbsolute();
}

void CButton::computeLocal() {
	TButtonParams& btParams = _states[_currentState];
	MAT44 tr = MAT44::CreateTranslation(btParams._params._position.x, btParams._params._position.y, 0.f);
	MAT44 rot = MAT44::CreateFromYawPitchRoll(0.f, 0.f, btParams._params._rotation);
	MAT44 sc = MAT44::CreateScale(btParams._params._scale.x, btParams._params._scale.y, 1.f);
	_local = rot * sc * tr;
}