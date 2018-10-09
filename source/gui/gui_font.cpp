#include "mcv_platform.h"
#include "gui_font.h"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace GUI {
	void CFont::load(std::string fileName) {
		file = fileName;
		std::string line;
		std::ifstream myfile(file, std::ios::in);
		if (myfile.is_open()) {
			//info face, size, bold, italic, charset, unicode, stretchH, smooth, aa, padding, spacing
			getline(myfile, line);
			line = line.substr(line.find("\"") + 1);
			face = line.substr(0, line.find("\""));
			line = line.substr(line.find("=") + 1);
			size = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			bold = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			italic = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("\"") + 1);
			charset = line.substr(0, line.find("\""));
			line = line.substr(line.find("=") + 1);
			unicode = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			stretchH = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			smooth = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			aa = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			padding[0] = std::stoi(line.substr(0, line.find(",")));
			line = line.substr(line.find(",") + 1);
			padding[1] = std::stoi(line.substr(0, line.find(",")));
			line = line.substr(line.find(",") + 1);
			padding[2] = std::stoi(line.substr(0, line.find(",")));
			line = line.substr(line.find(",") + 1);
			padding[3] = std::stoi(line.substr(0, line.find(",")));
			line = line.substr(line.find("=") + 1);
			spacing[0] = std::stoi(line.substr(0, line.find(",")));
			line = line.substr(line.find(",") + 1);
			spacing[1] = std::stoi(line.substr(0, line.find(",")));

			//lineHeight, base, scaleW, scaleH, pages, packed
			getline(myfile, line);
			line = line.substr(line.find("=") + 1);
			lineHeight = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			base = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			scaleW = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			scaleH = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			pages = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("=") + 1);
			packed = std::stoi(line.substr(0, line.find(" ")));

			//Page id, texture file
			getline(myfile, line);
			line = line.substr(line.find("=") + 1);
			pageId = std::stoi(line.substr(0, line.find(" ")));
			line = line.substr(line.find("\"") + 1);
			textureFile = line.substr(0, line.find("\""));

			//Chars count
			getline(myfile, line);
			line = line.substr(line.find("=") + 1);
			charsCount = std::stoi(line.substr(0, line.find(" ")));

			//chars
			for (int i = 0; i < charsCount; i++) {
				int id;
				CharInfo ci;
				getline(myfile, line);

				line = line.substr(line.find("=") + 1);
				id = std::stoi(line.substr(0, line.find(" ")));
				line = line.substr(line.find("=") + 1);
				ci.x = std::stoi(line.substr(0, line.find(" ")));
				line = line.substr(line.find("=") + 1);
				ci.y = std::stoi(line.substr(0, line.find(" ")));
				line = line.substr(line.find("=") + 1);
				ci.width = std::stoi(line.substr(0, line.find(" ")));
				line = line.substr(line.find("=") + 1);
				ci.height = std::stoi(line.substr(0, line.find(" ")));
				line = line.substr(line.find("=") + 1);
				ci.xoffset = std::stoi(line.substr(0, line.find(" ")));
				line = line.substr(line.find("=") + 1);
				ci.yoffset = std::stoi(line.substr(0, line.find(" ")));
				line = line.substr(line.find("=") + 1);
				ci.xadvance = std::stoi(line.substr(0, line.find(" ")));
				line = line.substr(line.find("=") + 1);
				ci.page = std::stoi(line.substr(0, line.find(" ")));
				line = line.substr(line.find("=") + 1);
				ci.chnl = std::stoi(line.substr(0, line.find(" ")));

				characters[id] = ci;
			}

			myfile.close();

			if (!textureFile.empty()) {
				fontTexture = Resources.get(textureFile)->as<CTexture>();
			}
		}
	}

	const CTexture* CFont::getTexture() {
		return fontTexture;
	}

	void CFont::renderText(const MAT44& world, const std::string& text, const VEC4& color) {
		assert(fontTexture);

		EngineGUI.getCamera().setPerspective(-1.f, 1.f, Render.width, Render.height);
		activateCamera(EngineGUI.getCamera(), Render.width, Render.height);

		VEC2 scale( Engine.globalConfig.resolution.x / 1920.f,
			Engine.globalConfig.resolution.y / 1080.f);

		MAT44 worldScaled = MAT44::CreateScale(size)
			* MAT44::CreateTranslation(world.Translation().x * scale.x, world.Translation().y * scale.y, world.Translation().z);

		VEC2 gap(0, 0);
		for (size_t i = 0; i < text.size(); ++i) {
			char c = text[i];
			if (c == '\n') {
				gap.x = 0.f;
				gap.y += lineHeight / (float)size;
			}
			else {
				VEC2 minUV = VEC2(characters[c].x / (float)scaleW, characters[c].y / (float)scaleH);
				VEC2 maxUV = minUV + VEC2((characters[c].width + characters[c].xoffset /*+ spacing[0]*/) / (float)scaleW,
					(characters[c].height + characters[c].yoffset + spacing[1]) / (float)scaleH);
				MAT44 w = MAT44::CreateTranslation(gap.x, gap.y, 0.f) * worldScaled;
				gap += VEC2(characters[c].xadvance / (float)size, 0.f);

				VEC2 charSize = VEC2((characters[c].width + characters[c].xoffset) / (float)size,
					(characters[c].height + characters[c].yoffset) / (float)size);

				EngineGUI.renderText(w, fontTexture, minUV, maxUV, color, charSize);
			}
		}

		EngineGUI.getCamera().setPerspective(-1.f, 1.f, 1920, 1080);
		activateCamera(EngineGUI.getCamera(), Render.width, Render.height);
	}

	int CFont::getSize() {
		return size;
	}

	int CFont::getHeight() {
		return size;
	}

	int CFont::getWidth(std::string text) {
		int size = 0;
		int lineSize = 0;

		for (size_t i = 0; i < text.size(); ++i) {
			if (text[i] == '\n') {
				size = std::max(size, lineSize);
				lineSize = 0;
			}
			else {
				lineSize += characters[text[i]].xadvance;
			}
		}
		size = std::max(size, lineSize);

		return size;
	}

	int CFont::getHeight(std::string text) {
		int lines = 0;

		if (!text.empty()) {
			lines = std::count(text.begin(), text.end(), '\n') + 1;
		}

		return lines * size;
	}
}
