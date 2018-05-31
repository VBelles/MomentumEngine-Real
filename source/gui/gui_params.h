#pragma once

#include "utils/template_engine.h"

namespace GUI {
	struct TParams {
		bool _visible = true;
		VEC2 _size;
		VEC2 _position;
		float _rotation = 0.f;
		VEC2 _scale;
	};

	struct TImageParams {
		const CTexture* _texture = nullptr;
		VEC4 _color;
		VEC2 _minUV;
		VEC2 _maxUV;
	};

	struct TTextParams {
		enum EAlignment { Left, Center, Right, Top, Bottom };
		EAlignment _vAlign = Left;
		EAlignment _hAlign = Top;
		float _size;
		//const CFont* _font = nullptr;
		std::string _text;
		VEC4 _color;
		VTemplate::TTemplate _templateText;
	};

	struct TBarParams {
		enum EDirection { Horizontal, Vertical };
		EDirection _direction;
		std::string _variable;
	};

	struct TButtonParams {
		TParams _params;
		TImageParams _imageParams;
		TTextParams _textParams;
	};

	struct TPowerBarParams {
		std::string _progressVariable;
		std::string _colorIndexVariable;
		std::vector<VEC4> _colors;
	};

	struct THpBarParams {
		std::string _hpVariable;
		std::string _maxHpVariable;
		VEC4 _color;
		VEC4 _colorEmpty;
	};

	using GUICallback = std::function<void()>;
}
