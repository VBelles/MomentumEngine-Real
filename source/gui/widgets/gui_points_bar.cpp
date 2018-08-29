#include "mcv_platform.h"
#include "gui_points_bar.h"
#include "gui/widgets/gui_point.h"

using namespace GUI;

void CPointsBar::renderAll() {
	int points = EngineGUI.getVariables().getInt(_variable);
	assert(points <= _children.size());
	int max = EngineGUI.getVariables().getInt(_variable_max, points);
	assert(max >= points && max <= _children.size());
	for (int i = 0; i < max; i++) {
		CPoint* point = dynamic_cast<CPoint*>(_children[i]);
		if (point) {
			if (i < points) {
				point->setCurrentState(CPoint::EState::ST_Full);
			}
			else {
				point->setCurrentState(CPoint::EState::ST_Empty);
			}
			point->renderAll();
		}
	}
}

TImageParams* CPointsBar::getImageParams() {
	return &_imageParams;
}
