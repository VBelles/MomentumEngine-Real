#pragma once

#include "gui/gui_widget.h"



namespace GUI {
	class CImage;
	class CHpBar : public CWidget {
	public:
		CHpBar() = default;
		void render() override;
		
	private:
		THpBarParams* hpBarParams;
		CImage* hpImage;
		friend class CParser;
	};
}