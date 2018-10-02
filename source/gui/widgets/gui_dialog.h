#pragma once

#include "gui/gui_widget.h"

namespace GUI {
	class CDialog : public CWidget {
	public:
		CDialog() = default;
		void render() override;
		TImageParams* getImageParams() override;
		TTextParams* getTextParams() override;

		void setText(const std::string& text, const int& size);

	private:
		TTextParams _textParams;
		TImageParams _imageParams;

		VEC4 padding = VEC4::Zero; //up, down, left, right

		friend class CParser;
	};
}
