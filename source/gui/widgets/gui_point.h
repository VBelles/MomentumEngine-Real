#pragma once

#include "gui/gui_widget.h"

namespace GUI {
	class CPoint : public CWidget {
	public:
		enum EState { ST_Full = 0, ST_Empty, NUM_STATES };

		CPoint() = default;
		void render() override;
		TImageParams* getImageParams() override;
		void setCurrentState(EState newState);

	private:
		EState _currentState = ST_Full;
		TImageParams _states[NUM_STATES];

		friend class CParser;
	};
}
