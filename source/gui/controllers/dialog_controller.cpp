#include "mcv_platform.h"
#include "dialog_controller.h"
#include "gui/gui_parser.h"
#include "gui/widgets/gui_dialog.h"

namespace GUI {
	CDialogController::CDialogController() {
		GUI::CParser parser;
		parser.parseFile("data/gui/dialog_box.json");
	}

	CDialogController::~CDialogController() {
		EngineGUI.unregisterWidget(widgetName);
	}

	void CDialogController::update(float delta) {}

	void CDialogController::showDialog(const std::string& text, const int& fontSize) {
		CDialog* dialogWidget = (CDialog*)EngineGUI.getWidget(widgetName);
		dialogWidget->setText(text, fontSize);
		EngineGUI.activateWidget(widgetName);
	}

	void CDialogController::hideDialog() {
		EngineGUI.deactivateWidget(widgetName);
	}
}
