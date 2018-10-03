#pragma once
#include "gui/gui_controller.h"
#include "gui/widgets/gui_dialog.h"

namespace GUI {
	class CDialogController : public CController {
	private:
		std::string widgetName = "dialog_box";
		CDialog* dialogWidget = nullptr;
		bool dialogActive = false;
	public:
		CDialogController();
		~CDialogController();

		virtual void update(float delta) override;
		void showDialog(const std::string& text, const int& fontSize = 16);
		void hideDialog();
		bool isActive();
	};
}
