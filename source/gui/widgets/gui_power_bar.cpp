#include "mcv_platform.h"
#include "gui_power_bar.h"

using namespace GUI;

void CPowerBar::render() {
	float ratio = EngineGUI.getVariables().getFloat(_powerBarParams._progressVariable);
	int colorIndex = EngineGUI.getVariables().getInt(_powerBarParams._colorIndexVariable);
	VEC4 color;
	if (colorIndex >= 0 && colorIndex < _powerBarParams._colors.size()) {
		color = _powerBarParams._colors[colorIndex];
	}
	ratio = clamp(ratio, 0.f, 1.f);
	MAT44 sz = MAT44::CreateScale(_params._size.x, _params._size.y, 1.f);
	MAT44 w = MAT44::CreateScale(ratio, 1.f, 1.f) * sz * _absolute;
	VEC2 maxUV = _imageParams._maxUV;
	maxUV.x *= ratio;
    EngineGUI.renderTexture(w,
		_imageParams._texture,
		_imageParams._minUV,
		maxUV,
		color);
}

TImageParams* CPowerBar::getImageParams() {
	return &_imageParams;
}
