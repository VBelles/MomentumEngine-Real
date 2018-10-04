#pragma once

#include "gui/gui_controller.h"

namespace GUI {
	class CWidget;

	class CHudController : public CController {
	private:
		std::string hudVariablesFile = "data/gui/hud_variables.json";

		CTimer2 timer;
		std::string chrysalisWidgetName = "chrysalis";
		CWidget* chrysalisWidget = nullptr;
		float chrysalisVisibleTime = 0.f;

	public:
		CHudController();
		virtual void update(float delta) override;

		void showChrysalis();
		void showChrysalis(float time);
		void hideChrysalis();
	};
}
