#pragma once

#include "gui/gui_widget.h"

namespace GUI {
	class CText : public CWidget {
	public:
		CText() = default;
		void render() override;
		TTextParams* getTextParams() override;

	private:
		TTextParams _textParams;
		VTemplate::VariableCallback variableCallback = [](const std::string& name) {
			CVariant* variant = EngineGUI.getVariables().getVariant(name);
			return variant ? variant->toString() : "";
		};

		friend class CParser;
	};
}