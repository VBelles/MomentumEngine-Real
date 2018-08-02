#pragma once

#include "gui/gui_controller.h"

namespace GUI {
	class CMapMarker;

	class CMapMenuController : public CController {
	public:
		virtual void update(float delta) override;
		void registerOption(const std::string& name);
		void setCurrentOption(int newOption);

	private:
		std::vector<CMapMarker*> _options;
		int _currentOption = 0;
		float stickInputCooldown = 7.f / 60;
		CTimer stickInputTimer;
	};
}
