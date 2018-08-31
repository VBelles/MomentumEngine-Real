#pragma once

#include "gui/gui_widget.h"

namespace GUI {
	class CImage : public CWidget {
	public:
		CImage() = default;
		void render() override;
		TImageParams* getImageParams() override;

	private:
		CTimer2 timer;
		TImageParams _imageParams;

		std::pair<VEC2, VEC2> getUV();

		friend class CParser;
	};
}
