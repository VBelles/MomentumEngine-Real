#pragma once

#include "gui/gui_effect.h"

namespace GUI {
	class CEllipsis : public CEffect {
	public:
		virtual void update(float delta) override;

	private:
		float waitTime = 1.f;
		CTimer timer;
		int nVisibles = 0.f;

		friend class CParser;
	};
}
