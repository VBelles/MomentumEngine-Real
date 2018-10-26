#pragma once
#include "gui/gui_controller.h"
#include "gui/widgets/gui_dialog.h"

namespace GUI {
	class CDialogController : public CController {
	private:
		std::string widgetName = "dialog_box";
		CDialog* dialogWidget = nullptr;
		bool dialogActive = false;
		bool visible = true;
		bool isCancelableWithButton = true;
	public:
		CDialogController();
		~CDialogController();

		virtual void update(float delta) override;
		void showDialog(const std::string& text, const int& fontSize = 16, bool cancelable = true);
		void hideDialog();
		bool isActive();
		void setVisible(bool newVisible);

		void setCancelableWithButton(bool cancelable);
	};
}
