#pragma once

namespace GUI {
	class CFont {
	private:
		struct CharInfo {
			int x = 0;
			int y = 0;
			int width = 0;
			int height = 0;
			int xoffset = 0;
			int yoffset = 0;
			int xadvance = 0;
			int page = 0;
			int chnl = 0;
		};

		std::string file = "";

		std::map<int, CharInfo> characters;

		std::string face = "Arial";
		int bold = 0;
		int italic = 0;
		std::string charset = "";
		int unicode = 0;
		int stretchH = 0;
		int smooth = 0;
		int aa = 0;
		int padding[4] = { 0, 0, 0, 0 };
		int spacing[2] = { 0, 0 };
		int lineHeight = 0;
		int base = 0;
		int scaleW = 0;
		int scaleH = 0;
		int pages = 0;
		int packed = 0;
		int pageId = 0;
		std::string textureFile = "";
		int charsCount = 0;

		const CTexture* fontTexture = nullptr;

	public:
		int size = 0;
		void load(std::string fileName);
		const CTexture* getTexture();
		void renderText(const MAT44& world, const std::string& text, const VEC4& color);
	};
}