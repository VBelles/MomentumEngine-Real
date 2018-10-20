#include "mcv_platform.h"
#include "gui_ellipsis.h"

namespace GUI {
	void CEllipsis::update(float delta) {
		VWidgets children = _widget->getChildren();
		if (timer.elapsed() > waitTime) {
			timer.reset();
			nVisibles++;
			if (nVisibles > children.size()) {
				nVisibles = 0;
			}
		}
		for (int i = 0; i < children.size(); i++) {
			if (i < nVisibles) {
				children[i]->getParams()->_visible = true;
			}
			else {
				children[i]->getParams()->_visible = false;
			}
		}
	}
}
