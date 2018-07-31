#pragma once

#include "gui/gui_controller.h"

namespace GUI {
	class COption;

	class COptionsMenuController : public CController {
	public:
		virtual void update(float delta) override;
		void registerOption(const std::string& name, GUICallback cb);
		void setCurrentOption(int newOption);

	private:
		std::vector<COption*> _options;
		int _currentOption = 0;
		float stickInputCooldown = 5.f / 60;
		CTimer stickInputTimer;
	};
}
