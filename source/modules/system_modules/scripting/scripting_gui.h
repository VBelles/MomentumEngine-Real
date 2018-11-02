#pragma once

namespace SLB {
	class Manager;
}

class ScriptingGUI {
private:
	static void bindConstants(SLB::Manager* manager);

public:
	static void bind(SLB::Manager* manager);

	static void showDialog(std::string text, int fontSize, bool cancelable);
	static void hideDialog();
	static bool isDialogActive();
	static bool isDialogHidden();
	static void setCancelableWithButton(bool cancelable);

	static void showHUD(bool how);
	static bool skipCinematics();

	static void goToMainMenu();
	static void goToCredits();
};
