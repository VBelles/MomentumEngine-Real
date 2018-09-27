#pragma once

#include "gui/gui_widget.h"

namespace GUI {
	class CButton;
	class CText;

	class COption : public CWidget {
	public:
		enum EState { ST_Idle = 0, ST_Selected, NUM_STATES };

		COption() = default;
		~COption();
		void render() override;
		TImageParams* getImageParams() override;
		TTextParams* getTextParams() override;
		void setCurrentState(EState newState);
		void setCurrentOption(int newOption);
		void setCurrentValue(json newValue);
		void computeAbsolute() override;

		int getCurrentOption();
		json getCurrentOptionValue();
		CButton* getPreviousButton();
		CButton* getNextButton();

	private:
		TOptionParams _states[NUM_STATES];
		EState _currentState = ST_Idle;

		CButton* _previous = nullptr;
		CButton* _next = nullptr;
		CText* _text = nullptr;

		int _currentOption;
		std::vector<std::pair<std::string, json>> _options;

		friend class CParser;
	};
}
