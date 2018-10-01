#pragma once
#include "gui/gui_controller.h"

namespace GUI {
	class CDialogController : public CController {
	private:
		std::string widgetName = "dialog_box";

	public:
		CDialogController();
		~CDialogController();

		virtual void update(float delta) override;
		void showDialog(const std::string& text, const int& fontSize = 16);
		void hideDialog();
	};
}
