#pragma once

#include "gui/gui_controller.h"
#include "gui/widgets/gui_button.h"

namespace GUI {
	class CMapMarker;

	class CMapMenuController : public CController {
	public:
		virtual void update(float delta) override;
		void registerOption(const std::string& name, GUICallback cb = nullptr);
		void unregisterOption(const std::string& name);
		void setCurrentOption(int newOption);
		std::string mapModule = "map_menu";
	private:
		struct TOption {
			CButton* button;
			GUICallback callback;
		};
		TOption showChrysalisOpt;

		std::vector<CMapMarker*> _options;
		int _currentOption = 0;
		float stickInputCooldown = 7.f / 60;
		CTimer stickInputTimer;

		void setState(int option, CButton::EState state);
		bool optionOverlaps(int option, VEC2 pos);
	};
}
