#include "mcv_platform.h"
#include "gui_option.h"
#include "gui/widgets/gui_text.h"
#include "gui/widgets/gui_button.h"

using namespace GUI;

COption::~COption() {
	SAFE_DELETE(_previous);
	SAFE_DELETE(_next);
	SAFE_DELETE(_text);
}


void COption::render() {
	TOptionParams& optionParams = _states[_currentState];

	// render image
	MAT44 sz = MAT44::CreateScale(_params._size.x, _params._size.y, 1.f);
	EngineGUI.renderTexture(sz * _absolute,
		optionParams._imageParams._texture,
		optionParams._imageParams._minUV,
		optionParams._imageParams._maxUV,
		optionParams._imageParams._color);

	// render text
	float textWidth = optionParams._textParams._text.size() * optionParams._textParams._size;
	float textHeight = optionParams._textParams._size;
	VEC2 textSpace = _params._size;
	textSpace.x -= _previous->getParams()->_size.x + _next->getParams()->_size.x + _text->getParams()->_size.x;
	VEC2 offset;
	if (optionParams._textParams._hAlign == TTextParams::Center)
		offset.x = textSpace.x * 0.5f - textWidth * 0.5f;
	else if (optionParams._textParams._hAlign == TTextParams::Right)
		offset.x = textSpace.x - textWidth;
	if (optionParams._textParams._vAlign == TTextParams::Center)
		offset.y = _params._size.y * 0.5f - textHeight * 0.5f;
	else if (optionParams._textParams._vAlign == TTextParams::Bottom)
		offset.y = _params._size.y - textHeight;

	MAT44 tr = MAT44::CreateTranslation(offset.x, offset.y, 0.f);
	MAT44 w = MAT44::CreateScale(optionParams._textParams._size) * tr * _absolute;
	EngineGUI.renderText(w, optionParams._textParams._text, optionParams._textParams._color);

	// render previous button
	_previous->render();

	//render option text
	_text->render();

	// render next button
	_next->render();
}

TImageParams* COption::getImageParams() {
	return &_states[_currentState]._imageParams;
}

TTextParams* COption::getTextParams() {
	return &_states[_currentState]._textParams;
}

void COption::setCurrentState(EState newState) {
	_currentState = newState;
}

void COption::setCurrentOption(int newOption) {
	int nOptions = static_cast<int>(_options.size());
	_currentOption = (newOption + nOptions) % nOptions;
	_text->getTextParams()->_text = _options[_currentOption];
}

void COption::computeAbsolute() {
	CWidget::computeAbsolute();
	_previous->computeAbsolute();
	_text->computeAbsolute();
	_next->computeAbsolute();
}

int COption::getCurrentOption() {
	return _currentOption;
}