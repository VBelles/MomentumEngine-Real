#include "mcv_platform.h"
#include "gui_text.h"

using namespace GUI;

void CText::render() {
	if (!_textParams._templateText.texts.empty()) {
		_textParams._text = _textParams._templateText.toString(variableCallback);
	}

	CFont font = EngineGUI.getFont(_textParams._size);

	float textWidth = font.getWidth(_textParams._text);
	float textHeight = font.getHeight(_textParams._text);

	VEC2 textSpace = _params._size;
	VEC2 offset;
	if (_textParams._hAlign == TTextParams::Center)
		offset.x = textSpace.x * 0.5f - textWidth * 0.5f;
	else if (_textParams._hAlign == TTextParams::Right)
		offset.x = textSpace.x - textWidth;
	if (_textParams._vAlign == TTextParams::Center)
		offset.y = _params._size.y * 0.5f - textHeight * 0.5f;
	else if (_textParams._vAlign == TTextParams::Bottom)
		offset.y = _params._size.y - textHeight;

	MAT44 tr = MAT44::CreateTranslation(offset.x, offset.y, 0.f);
	MAT44 w = MAT44::CreateScale(font.getSize()) * tr * _absolute;
	font.renderText(w, _textParams._text, _textParams._color);
}

TTextParams* CText::getTextParams() {
	return &_textParams;
}

