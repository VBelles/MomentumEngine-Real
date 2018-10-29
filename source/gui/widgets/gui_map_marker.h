#pragma once

#include "gui/gui_widget.h"
#include "gui/widgets/gui_button.h"

namespace GUI {

	class CMapMarker : public CWidget {
	public:
		CMapMarker() = default;
		~CMapMarker();
		void render() override;
		void renderAll() override;
		void setCurrentState(CButton::EState newState);
		void computeAbsolute() override;
		bool overlaps(VEC2 pos) override;
		bool isVisible();
		void setVisible(bool visible);

		CButton* getButton();
		CButton* getMarker();
		VEC3 getPos();

	private:
		VEC3 pos = VEC3::Zero;
		bool _visible = false;
		std::string mapWidget = "";

		CButton* _alternButton = nullptr;
		CButton* _button = nullptr;
		CButton* _marker = nullptr;

		friend class CParser;
	};
}
