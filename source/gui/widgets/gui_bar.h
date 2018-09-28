#pragma once

#include "gui/gui_widget.h"

namespace GUI {
	class CBar : public CWidget {
	public:
		CBar() = default;
		void render() override;
		TImageParams* getImageParams() override;

	private:
		TImageParams _imageParams;
		TBarParams _barParams;

		CTimer2 timer;
		std::pair<VEC2, VEC2> getUV();

		friend class CParser;
	};
}
