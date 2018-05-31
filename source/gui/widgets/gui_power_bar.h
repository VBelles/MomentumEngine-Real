#pragma once

#include "gui/gui_widget.h"

namespace GUI {
	class CPowerBar : public CWidget {
	public:
		CPowerBar() = default;
		void render() override;
		TImageParams* getImageParams() override;

	private:
		TImageParams _imageParams;
		TPowerBarParams _powerBarParams;

		friend class CParser;
	};
}