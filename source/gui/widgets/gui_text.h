#pragma once

#include "gui/gui_widget.h"

namespace GUI {
	class CText : public CWidget {
	public:
		CText() = default;
		void render() override;
		TTextParams* getTextParams() override;
		void updateTemplate();

	private:
		TTextParams _textParams;

		friend class CParser;
	};
}