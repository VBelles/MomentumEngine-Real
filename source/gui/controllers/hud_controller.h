#pragma once

#include "gui/gui_controller.h"

namespace GUI {
	class CHudController : public CController {
	private:
	public:
		CHudController();
		virtual void update(float delta) override;
	};
}
