#pragma once

#include "gui/gui_widget.h"

namespace GUI {
	class CPointsBar : public CWidget {
	public:
		CPointsBar() = default;
		void renderAll() override;
		TImageParams* getImageParams() override;

	private:
		std::string _variable;
		std::string _variable_max;
		TImageParams _imageParams;

		friend class CParser;
	};
}
