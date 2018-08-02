#pragma once

#include "gui/gui_controller.h"

namespace GUI {
	class CButton;

	class CMainMenuController : public CController {
	public:
		virtual void update(float delta) override;
		void registerOption(const std::string& name, GUICallback cb);
		void setCurrentOption(int newOption);

	private:
		struct TOption {
			CButton* button;
			GUICallback callback;
		};
		std::vector<TOption> _options;
		int _currentOption = 0;
		float stickInputCooldown = 7.f / 60;
		CTimer stickInputTimer;
	};
}
