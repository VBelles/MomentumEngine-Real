#include "mcv_platform.h"
#include "gui_dialog.h"
#include <sstream>

using namespace GUI;

void CDialog::render() {
	// render image
	MAT44 sz = MAT44::CreateScale(_params._size.x, _params._size.y, 1.f);
	EngineGUI.renderTexture(sz * _absolute,
		_imageParams._texture,
		_imageParams._minUV,
		_imageParams._maxUV,
		_imageParams._color);

	// render text
	CFont font = EngineGUI.getFont(_textParams._size);

	float textWidth = font.getWidth(_textParams._text);
	float textHeight = font.getHeight(_textParams._text);

	VEC2 textSpace = _params._size;
	VEC2 offset = VEC2(padding.z, padding.x);
	if (_textParams._hAlign == TTextParams::Center)
		offset.x += textSpace.x * 0.5f - textWidth * 0.5f;
	else if (_textParams._hAlign == TTextParams::Right)
		offset.x = textSpace.x - textWidth - padding.w;
	if (_textParams._vAlign == TTextParams::Center)
		offset.y += textSpace.y * 0.5f - textHeight * 0.5f;
	else if (_textParams._vAlign == TTextParams::Bottom)
		offset.y = textSpace.y - textHeight - padding.y;

	MAT44 tr = MAT44::CreateTranslation(offset.x, offset.y, 0.f);
	MAT44 w = MAT44::CreateScale(font.getSize()) * tr * _absolute;
	font.renderText(w, _textParams._text, _textParams._color);
}

TImageParams* CDialog::getImageParams() {
	return &_imageParams;
}

TTextParams* CDialog::getTextParams() {
	return &_textParams;
}

void split(const std::string& s, char delim, std::vector<std::string>& elems) {
	std::stringstream ss(s);
	std::string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void CDialog::setText(const std::string& text, const int& size) {
	_textParams._size = size;
	_textParams._text = "";

	CFont font = EngineGUI.getFont(_textParams._size);
	float textWidth = font.getWidth(text);
	VEC2 textSpace = VEC2(_params._size.x - padding.z - padding.w, _params._size.y - padding.x - padding.y);

	if (textWidth > textSpace.x) {
		size_t delimiterPos = text.find(" ");
		std::string line = text.substr(0, delimiterPos);
		std::vector<std::string> words = split(text.substr(delimiterPos + 1), ' ');
		for (std::string word : words) {
			if (font.getWidth(line + word) > textSpace.x) {
				_textParams._text += line + "\n";
				line = word;
			}
			else {
				line += " " + word;
			}
		}
		_textParams._text += line;
	}
	else {
		_textParams._text = text;
	}
}