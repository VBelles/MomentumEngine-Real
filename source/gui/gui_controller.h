#pragma once

namespace GUI {
	class CController {
	public:
		virtual void update(float delta) { (void)delta; }
	};

	using VControllers = std::vector<CController*>;
}
