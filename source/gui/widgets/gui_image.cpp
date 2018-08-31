#include "mcv_platform.h"
#include "gui/widgets/gui_image.h"

using namespace GUI;

void CImage::render() {
	MAT44 sz = MAT44::CreateScale(_params._size.x, _params._size.y, 1.f);

	std::pair<VEC2, VEC2> UVs = getUV();
	VEC2 minUV = UVs.first;
	VEC2 maxUV = UVs.second;

	EngineGUI.renderTexture(sz * _absolute,
		_imageParams._texture,
		minUV,
		maxUV,
		_imageParams._color,
		_imageParams._mask);
}

TImageParams* CImage::getImageParams() {
	return &_imageParams;
}

std::pair<VEC2, VEC2> CImage::getUV() {
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
