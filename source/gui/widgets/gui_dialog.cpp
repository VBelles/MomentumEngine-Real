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
	VEC2 scale(Engine.globalConfig.resolution.x / EngineGUI.getResolution().x,
		Engine.globalConfig.resolution.y / EngineGUI.getResolution().y);
	CFont font = EngineGUI.getFont(_textParams._size * scale.y);

	float textWidth = font.getWidth(_textParams._text);
	float textHeight = font.getHeight(_textParams._text);

	VEC2 textSpace = _params._size;
	VEC2 offset = VEC2(padding.z, padding.x);
	if (_textParams._hAlign == TTextParams::Center)
		offset.x = textSpace.x * 0.5f - textWidth * 0.5f / scale.x;
	else if (_textParams._hAlign == TTextParams::Right)
		offset.x = textSpace.x - textWidth - padding.w;
	if (_textParams._vAlign == TTextParams::Center)
		offset.y = textSpace.y * 0.5f - textHeight * 0.5f / scale.y;
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
	textFragments.clear();
	currentFragment = 0;

	_textParams._size = size;
	_textParams._text = "";

	VEC2 scale(Engine.globalConfig.resolution.x / EngineGUI.getResolution().x,
		Engine.globalConfig.resolution.y / EngineGUI.getResolution().y);

	CFont font = EngineGUI.getFont(_textParams._size * scale.y);
	VEC2 textSpace = VEC2((_params._size.x - padding.z - padding.w) * scale.x,
		(_params._size.y - padding.x - padding.y) * scale.y);

	if (font.getWidth(text) > textSpace.x || font.getHeight(text) > textSpace.y) {
		std::string textFragment = "";
		std::string line = "";
		std::vector<std::string> words = split(text, ' ');
		for (int i = 0; i < words.size(); i++) {
			std::string word = words[i];
			if (font.getWidth(line + word) > textSpace.x) {
				if (line == "\n") line = "";
				if (font.getHeight(textFragment + line) > textSpace.y) {
					textFragments.push_back(textFragment);
					textFragment = line + "\n";
				}
				else if (!line.empty()) {
					textFragment += line + "\n";
				}
				line = "";
			}

			if (i > 0 && !line.empty()) line += " ";
			line += word;

			if (font.getHeight(textFragment + line) > textSpace.y) {
				std::vector<std::string> lines = split(line, '\n');
				if (lines.size() > 1) {
					for (int j = 0; j < lines.size(); j++) {
						std::string s = lines[j];
						if (font.getHeight(textFragment + s) > textSpace.y) {
							textFragments.push_back(textFragment);
							textFragment = "";
						}
						textFragment += s;
						if (j != lines.size() - 1) textFragment += "\n";
					}
					line = "";
				}
				else {
					textFragments.push_back(textFragment);
					textFragment = "";
				}
			}
		}
		textFragment += line;
		if (!textFragment.empty()) {
			textFragments.push_back(textFragment);
		}
	}
	else {
		textFragments.push_back(text);
	}

	for (auto it = textFragments.begin(); it != textFragments.end();) {
		if ((*it).empty() || (*it) == "\n") {
			it = textFragments.erase(it);
		}
		else {
			++it;
		}
	}

	_textParams._text = textFragments[currentFragment];
}

void CDialog::showNext() {
	if (currentFragment < textFragments.size() - 1) currentFragment++;
	_textParams._text = textFragments[currentFragment];
}

int CDialog::getNumFragments() {
	return textFragments.size();
}

int CDialog::getCurrentFragment() {
	return currentFragment;
}