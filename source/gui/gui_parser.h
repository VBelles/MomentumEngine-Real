#pragma once

#include "gui_params.h"

namespace GUI {
	class CImageWidget;
	class CEffect;

	class CParser {
	public:
		void parseFile(const std::string& file);
		CWidget* parseWidget(const json& data, CWidget* parent);

		CWidget* parseWidget(const json& data);
		CWidget* parseImage(const json& data);
		CWidget* parseText(const json& data);
		CWidget* parseButton(const json& data);
		CWidget* parseBar(const json& data);
		CWidget* parseOption(const json& data);
		CWidget* parseMapMarker(const json& data, const std::string& name);
		CWidget* parsePointsBar(const json& data);
		CWidget* parsePoint(const json& data);
		CWidget* parseDialog(const json& data);

		CEffect* parseEffect(const json& data, CWidget* wdgt);
		CEffect* parseAnimateUVEffect(const json& data);
		CEffect* parseEllipsisEffect(const json& data);

		void parseParams(TParams& params, const json& data);
		void parseImageParams(TImageParams& params, const json& data);
		void parseTextParams(TTextParams& params, const json& data);
		void parseBarParams(TBarParams& params, const json& data);
	};
}
