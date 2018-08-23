#include "mcv_platform.h"
#include "gui_bar.h"

using namespace GUI;

void CBar::render() {
	float ratio = EngineGUI.getVariables().getFloat(_barParams._variable);
	ratio = clamp(ratio, 0.f, 1.f);
	MAT44 sz = MAT44::CreateScale(_params._size.x, _params._size.y, 1.f);
	MAT44 w;
	VEC2 maxUV = _imageParams._maxUV;
	VEC2 minUV = _imageParams._minUV;

	if (_barParams._direction == TBarParams::EDirection::Horizontal) {
		w = MAT44::CreateScale(ratio, 1.f, 1.f) * sz * _absolute;
		maxUV.x *= ratio;
	}
	else if (_barParams._direction == TBarParams::EDirection::Vertical) {
		w = MAT44::CreateScale(1.f, -ratio, 1.f) * sz * _absolute * MAT44::CreateTranslation(0.f, _params._size.y, 0.f);
		maxUV.y *= ratio;
	}

	EngineGUI.renderTexture(w,
		_imageParams._texture,
		minUV,
		maxUV,
		_imageParams._color);
}

TImageParams* CBar::getImageParams() {
	return &_imageParams;
}
