#pragma once

namespace SLB {
	class Manager;
}

class ScriptingGUI {
private:
	static void bindConstants(SLB::Manager* manager);

public:
	static void bind(SLB::Manager* manager);

	static void showDialog(std::string text, int fontSize);
	static void hideDialog();
};
