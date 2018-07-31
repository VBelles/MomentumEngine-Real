#pragma once

#include "gui/gui_controller.h"

namespace GUI {
	class COption;

	class COptionsMenuController : public CController {
	public:
		virtual void update(float delta) override;
		void registerOption(const std::string& name, const json& value = json());
		void setCurrentOption(int newOption);
		json getOptionValue(const std::string& name);

	private:
		std::vector<COption*> _options;
		int _currentOption = 0;
		float stickInputCooldown = 7.f / 60;
		CTimer stickInputTimer;
	};
}
