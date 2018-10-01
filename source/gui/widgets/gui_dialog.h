#pragma once

#include "gui/gui_widget.h"

namespace GUI {
	class CDialog : public CWidget {
	public:
		CDialog() = default;
		void render() override;
		TImageParams* getImageParams() override;
		TTextParams* getTextParams() override;

	private:
		TTextParams _textParams;
		TImageParams _imageParams;

		friend class CParser;
	};
}
