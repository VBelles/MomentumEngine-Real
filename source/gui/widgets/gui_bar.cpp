#include "mcv_platform.h"
#include "gui_bar.h"

using namespace GUI;

void CBar::render() {
	float ratio = EngineGUI.getVariables().getFloat(_barParams._variable);
	ratio = clamp(ratio, 0.f, 1.f);
	MAT44 sz = MAT44::CreateScale(_params._size.x, _params._size.y, 1.f);
	MAT44 w;
	std::pair<VEC2, VEC2> UVs = getUV();
	VEC2 minUV = UVs.first;
	VEC2 maxUV = UVs.second;

	if (_barParams._direction == TBarParams::EDirection::Horizontal) {
		w = MAT44::CreateScale(ratio, 1.f, 1.f) * sz * _absolute;
		maxUV.x *= ratio;
	}
	else if (_barParams._direction == TBarParams::EDirection::Vertical) {
		w = MAT44::CreateScale(1.f, ratio, 1.f) * sz * _absolute * MAT44::CreateTranslation(0.f, _params._size.y * (1.f - ratio), 0.f);
		minUV.y += (maxUV.y - minUV.y) * (1.f - ratio);
	}

	EngineGUI.renderTexture(w,
		_imageParams._texture,
		minUV,
		maxUV,
		_imageParams._color,
		_imageParams._mask);
}

TImageParams* CBar::getImageParams() {
	return &_imageParams;
}

std::pair<VEC2, VEC2> CBar::getUV() {
	VEC2 minUV = _imageParams._minUV;
	VEC2 maxUV = _imageParams._maxUV;

	if (_imageParams._numFrames > 0) {
		int frame_idx = (int)(timer.elapsed() * _imageParams._frameSpeed);
		int frame = _imageParams._initialFrame + (frame_idx % _imageParams._numFrames);

		const int frameCols = static_cast<int>(1.f / _imageParams._frameSize.x);

		int row = frame / frameCols;
		int col = frame % frameCols;
		minUV = VEC2(col * _imageParams._frameSize.x, row * _imageParams._frameSize.y);
		maxUV = minUV + _imageParams._frameSize;
	}

	return std::make_pair(minUV, maxUV);
}