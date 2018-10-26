#include "mcv_platform.h"
#include "dialog_controller.h"
#include "gui/gui_parser.h"

namespace GUI {
	CDialogController::CDialogController() {
		GUI::CParser parser;
		parser.parseFile("data/gui/dialog_box.json");
		dialogWidget = (CDialog*)EngineGUI.getWidget(widgetName);
		dialogActive = false;
	}

	CDialogController::~CDialogController() {
		EngineGUI.unregisterWidget(widgetName);
	}

	void CDialogController::update(float delta) {
		if (isCancelableWithButton && (EngineInput["menu_accept"].getsPressed() || EngineInput["next_dialog"].getsPressed())) {
			if (dialogActive) {
				if (dialogWidget->getCurrentFragment() < dialogWidget->getNumFragments() - 1) {
					dialogWidget->showNext();
				}
				else {
					hideDialog();
				}
			}
		}
	}

	void CDialogController::showDialog(const std::string& text, const int& fontSize, bool cancelable) {
		hideDialog();
		dialogWidget->setText(text, fontSize);
		EngineGUI.activateWidget(widgetName);
		dialogActive = true;
		visible = true;
		isCancelableWithButton = cancelable;
	}

	void CDialogController::hideDialog() {
		EngineGUI.deactivateWidget(widgetName);
		dialogActive = false;
		visible = false;
	}

	bool CDialogController::isActive() {
		return dialogActive;
	}

	void CDialogController::setVisible(bool newVisible) {
		if (visible != newVisible) {
			visible = newVisible;
			if (visible && dialogActive) {
				EngineGUI.activateWidget(widgetName);
			}
			else if (!visible) {
				EngineGUI.deactivateWidget(widgetName);
			}
		}
	}

	void CDialogController::setCancelableWithButton(bool cancelable) {
		isCancelableWithButton = cancelable;
	}
}
