#include "mcv_platform.h"
#include "gui/gui_parser.h"
#include "gui/widgets/gui_image.h"
#include "gui/widgets/gui_text.h"
#include "gui/widgets/gui_bar.h"
#include "gui/widgets/gui_button.h"
#include "utils/template_engine.h"
#include "gui/widgets/gui_option.h"
#include "gui/widgets/gui_map_marker.h"
#include "gui/widgets/gui_points_bar.h"
#include "gui/widgets/gui_point.h"
#include "gui/widgets/gui_dialog.h"
#include "gui/effects/gui_animate_uv.h"
#include "gui/effects/gui_ellipsis.h"
#include "modules/system_modules/module_uniques.h"

namespace {
	json mergeJson(const json& j1, const std::string& key) {
		json newData = j1;
		if (j1.count(key) > 0) {
			const json& j2 = j1[key];
			for (const auto &j : json::iterator_wrapper(j2)) {
				newData[j.key()] = j.value();
			}
		}
		return newData;
	}
}

using namespace GUI;

void CParser::parseFile(const std::string& filename) {
	std::vector<CWidget*> widgets;

	std::ifstream file_json(filename);
	json json_data;
	file_json >> json_data;

	for (auto& j_element : json_data) {
		CWidget* wdgt = parseWidget(j_element, nullptr);

		// computeAbsolute will propagate calculations to the children recursively
		wdgt->computeAbsolute();

		// register the widget within the manager
		Engine.getGUI().registerWidget(wdgt);

		widgets.push_back(wdgt);
	}

	for (auto& widget : widgets) {
		widget->computeAbsolute();
	}
}

CWidget* CParser::parseWidget(const json& data, CWidget* parent) {
	const std::string name = data.value("name", "");
	const std::string type = data.value("type", "widget");
	CWidget* wdgt = nullptr;

	// create and parse the widget
	if (type == "image")        wdgt = parseImage(data);
	else if (type == "text")    wdgt = parseText(data);
	else if (type == "bar")     wdgt = parseBar(data);
	else if (type == "button")  wdgt = parseButton(data);
	else if (type == "option")  wdgt = parseOption(data);
	else if (type == "map_marker")  wdgt = parseMapMarker(data, name);
	else if (type == "points_bar")  wdgt = parsePointsBar(data);
	else if (type == "point")  wdgt = parsePoint(data);
	else if (type == "dialog")  wdgt = parseDialog(data);
	else                        wdgt = parseWidget(data);

	wdgt->_name = name;

	// create and parse effects
	if (data.count("effects") > 0) {
		for (auto& fxData : data["effects"]) {
			parseEffect(fxData, wdgt);
		}
	}

	// add to parent
	if (parent) {
		parent->addChild(wdgt);
	}

	// create and parse children
	if (data.count("children") > 0) {
		for (auto& child : data["children"]) {
			parseWidget(child, wdgt);
		}
	}

	return wdgt;
}

// ------------- WIDGETS
CWidget* CParser::parseWidget(const json& data) {
	CWidget* wdgt = new CWidget();

	parseParams(wdgt->_params, data);

	return wdgt;
}

CWidget* CParser::parseImage(const json& data) {
	CImage* wdgt = new CImage();

	parseParams(wdgt->_params, data);
	parseImageParams(wdgt->_imageParams, data);

	return wdgt;
}

CWidget* CParser::parseText(const json& data) {
	CText* wdgt = new CText();

	parseParams(wdgt->_params, data);
	parseTextParams(wdgt->_textParams, data);

	return wdgt;
}

CWidget* CParser::parseButton(const json& data) {
	CButton* wdgt = new CButton();

	parseParams(wdgt->_params, data);
	parseParams(wdgt->_states[CButton::EState::ST_Idle]._params, data);
	parseImageParams(wdgt->_states[CButton::EState::ST_Idle]._imageParams, data);
	parseTextParams(wdgt->_states[CButton::EState::ST_Idle]._textParams, data);

	json jSelected = mergeJson(data, "selected");
	parseParams(wdgt->_states[CButton::EState::ST_Selected]._params, jSelected);
	parseImageParams(wdgt->_states[CButton::EState::ST_Selected]._imageParams, jSelected);
	parseTextParams(wdgt->_states[CButton::EState::ST_Selected]._textParams, jSelected);

	json jPressed = mergeJson(data, "pressed");
	parseParams(wdgt->_states[CButton::EState::ST_Pressed]._params, jPressed);
	parseImageParams(wdgt->_states[CButton::EState::ST_Pressed]._imageParams, jPressed);
	parseTextParams(wdgt->_states[CButton::EState::ST_Pressed]._textParams, jPressed);

	return wdgt;
}

CEffect* CParser::parseEffect(const json& data, CWidget* wdgt) {
	const std::string type = data.value("type", "");
	CEffect* fx = nullptr;

	// create and parse the widget
	if (type == "animate_uv")   fx = parseAnimateUVEffect(data);
	else if (type == "ellipsis")   fx = parseEllipsisEffect(data);

	// add to parent
	if (wdgt && fx) {
		wdgt->addEffect(fx);
	}

	return fx;
}

CEffect* CParser::parseAnimateUVEffect(const json& data) {
	CAnimateUV* fx = new CAnimateUV();

	fx->_speed = loadVEC2(data.value("speed", "0 0"));

	return fx;
}

CEffect* CParser::parseEllipsisEffect(const json& data) {
	CEllipsis* fx = new CEllipsis();

	fx->waitTime = data.value("wait_time", fx->waitTime);

	return fx;
}

CWidget* CParser::parseBar(const json& data) {
	CBar* wdgt = new CBar();

	parseParams(wdgt->_params, data);
	parseImageParams(wdgt->_imageParams, data);
	parseBarParams(wdgt->_barParams, data);

	return wdgt;
}

CWidget* CParser::parseOption(const json& data) {
	COption* wdgt = new COption();

	parseParams(wdgt->_params, data);
	json jIdle = mergeJson(data, "idle");
	parseParams(wdgt->_states[CButton::EState::ST_Idle]._params, jIdle);
	parseImageParams(wdgt->_states[CButton::EState::ST_Idle]._imageParams, jIdle);
	parseTextParams(wdgt->_states[CButton::EState::ST_Idle]._textParams, jIdle);

	json jSelected = mergeJson(data, "selected");
	parseParams(wdgt->_states[CButton::EState::ST_Selected]._params, jSelected);
	parseImageParams(wdgt->_states[CButton::EState::ST_Selected]._imageParams, jSelected);
	parseTextParams(wdgt->_states[CButton::EState::ST_Selected]._textParams, jSelected);

	json jPrevious = data["previous_button"];
	wdgt->_previous = (CButton*)parseButton(jPrevious);
	wdgt->_previous->_parent = wdgt;
	wdgt->_previous->getTextParams()->_hAlign = TTextParams::EAlignment::Center;
	wdgt->_previous->getTextParams()->_vAlign = TTextParams::EAlignment::Center;

	json jNext = data["next_button"];
	wdgt->_next = (CButton*)parseButton(jNext);
	wdgt->_next->_parent = wdgt;
	wdgt->_next->getTextParams()->_hAlign = TTextParams::EAlignment::Center;
	wdgt->_next->getTextParams()->_vAlign = TTextParams::EAlignment::Center;

	json jOptionText = data["option_text"];
	wdgt->_text = (CText*)parseText(jOptionText);
	wdgt->_text->_parent = wdgt;
	wdgt->_text->getTextParams()->_hAlign = TTextParams::EAlignment::Center;
	wdgt->_text->getTextParams()->_vAlign = TTextParams::EAlignment::Center;

	wdgt->_next->getParams()->_position.x = wdgt->_params._size.x - wdgt->_next->getParams()->_size.x;
	wdgt->_text->getParams()->_position.x = wdgt->_next->getParams()->_position.x - wdgt->_text->getParams()->_size.x;
	wdgt->_previous->getParams()->_position.x = wdgt->_text->getParams()->_position.x - wdgt->_previous->getParams()->_size.x;
	for (int i = 0; i < CButton::EState::NUM_STATES; i++) {
		wdgt->_next->getButtonParams(static_cast<CButton::EState>(i))->_params._position.x = wdgt->_params._size.x - wdgt->_next->getParams()->_size.x;
		wdgt->_previous->getButtonParams(static_cast<CButton::EState>(i))->_params._position.x = wdgt->_text->getParams()->_position.x - wdgt->_previous->getParams()->_size.x;
	}

	for (auto& jOption : data["options"]) {
		std::string text = jOption[0];

		wdgt->_options.push_back(std::make_pair(text, jOption[1]));
	}

	wdgt->setCurrentOption(data.value("default_option", 0));

	return wdgt;
}

CWidget* CParser::parseMapMarker(const json& data, const std::string& name) {
	CMapMarker* wdgt = new CMapMarker();

	if (data.count("position")) {
		wdgt->pos = loadVEC3(data["position"]);
	}
	wdgt->mapWidget = data.value("map", wdgt->mapWidget);
	wdgt->alternText = data.value("altern_text", wdgt->alternText);
	UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
	if (uniqueEvent) {
		wdgt->_visible = uniqueEvent->done;
	}
	else {
		wdgt->_visible = data.value("visible", wdgt->_visible);
	}

	json jButton = data["button"];
	wdgt->_button = (CButton*)parseButton(jButton);
	wdgt->_button->_parent = wdgt;

	json jMarker = data["marker"];
	wdgt->_marker = (CButton*)parseButton(jMarker);

	return wdgt;
}

CWidget* CParser::parsePointsBar(const json& data) {
	CPointsBar* wdgt = new CPointsBar();

	parseParams(wdgt->_params, data);
	wdgt->_variable = data.value("variable", "");
	wdgt->_variable_max = data.value("variable_max", "");

	return wdgt;
}

CWidget* CParser::parsePoint(const json& data) {
	CPoint* wdgt = new CPoint();

	parseParams(wdgt->_params, data);
	parseImageParams(wdgt->_states[CPoint::EState::ST_Full], data);

	json jEmpty = mergeJson(data, "empty");
	parseImageParams(wdgt->_states[CPoint::EState::ST_Empty], jEmpty);

	return wdgt;
}

CWidget* CParser::parseDialog(const json& data) {
	CDialog* wdgt = new CDialog();

	parseParams(wdgt->_params, data);
	parseImageParams(wdgt->_imageParams, data);
	parseTextParams(wdgt->_textParams, data);

	wdgt->padding = loadVEC4(data.value("padding", "0 0 0 0"));

	return wdgt;
}

void CParser::parseParams(TParams& params, const json& data) {
	params._visible = data.value("visible", true);
	params._position = loadVEC2(data.value("position", "0 0"));
	params._rotation = deg2rad(data.value("rotation", 0.f));
	params._scale = loadVEC2(data.value("scale", "1 1"));
	params._size = loadVEC2(data.value("size", "1 1"));
}

void CParser::parseImageParams(TImageParams& params, const json& data) {
	params._color = loadVEC4(data.value("color", "1 1 1 1"));
	params._minUV = loadVEC2(data.value("minUV", "0 0"));
	params._maxUV = loadVEC2(data.value("maxUV", "1 1"));
	const std::string textureFile = data.value("texture", "");
	if (!textureFile.empty()) params._texture = Resources.get(textureFile)->as<CTexture>();
	const std::string maskFile = data.value("mask", "");
	if (!maskFile.empty()) params._mask = Resources.get(maskFile)->as<CTexture>();
	params._initialFrame = data.value("initial_frame", 1);
	params._frameSize = loadVEC2(data.value("frame_size", "1 1"));;
	params._numFrames = data.value("num_frames", 0);
	params._frameSpeed = data.value("frame_speed", 0);
}

void CParser::parseTextParams(TTextParams& params, const json& data) {
	params._color = loadVEC4(data.value("font_color", "1 1 1 1"));
	params._size = data.value("font_size", 1.f);
	if (data.count("template_text")) {
		params._text = data.value("template_text", "");
		VTemplate::compileTemplate(params._text, &params._templateText);
	}
	else {
		params._text = data.value("text", "");
	}
	const std::string& hAlign = data.value("halign", "");
	if (hAlign == "center")      params._hAlign = TTextParams::Center;
	else if (hAlign == "right")  params._hAlign = TTextParams::Right;
	else                         params._hAlign = TTextParams::Left;
	const std::string& vAlign = data.value("valign", "");
	if (vAlign == "center")      params._vAlign = TTextParams::Center;
	else if (vAlign == "bottom") params._vAlign = TTextParams::Bottom;
	else                         params._vAlign = TTextParams::Top;
}

void CParser::parseBarParams(TBarParams& params, const json& data) {
	params._variable = data.value("variable", "");
	const std::string direction = data.value("direction", "horizontal");
	if (direction == "vertical") params._direction = TBarParams::Vertical;
	else if (direction == "vertical_anim") params._direction = TBarParams::VerticalAnim;
	else params._direction = TBarParams::Horizontal;
}