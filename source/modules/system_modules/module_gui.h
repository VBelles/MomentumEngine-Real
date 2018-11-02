#pragma once

#include "modules/module.h"
#include "gui/gui_widget.h"
#include "utils/variant.h"
#include "gui/gui_controller.h"
#include "camera/camera_gui.h"
#include "gui/gui_font.h"
#include "gui/controllers/dialog_controller.h"

class CCameraOrthographic;

class CModuleGUI : public IModule {
public:
	CModuleGUI(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void renderGUI() override;

	// widget management
	void registerWidget(GUI::CWidget* wdgt);
	void unregisterWidget(std::string name, bool recursive = false);
	void clearWidgets();
	GUI::CWidget* getWidget(const std::string& name, bool recursive = false) const;
	void activateWidget(const std::string& name);
	void deactivateWidget(const std::string& name);

	// controller management
	void registerController(GUI::CController* controller);
	void unregisterController(GUI::CController* controller);

	CCameraGui& getCamera();
	MVariants& getVariables();

	void renderTexture(const MAT44& world, const CTexture* texture, const VEC2& minUV, const VEC2& maxUV,
		const VEC4& color, const CTexture* mask = nullptr, const VEC2& maskMinUV = VEC2::Zero, const VEC2& maskMaxUV = VEC2::One,
		const std::string& tech = "");
	void renderText(const MAT44& world, const CTexture* texture, const VEC2& minUV, const VEC2& maxUV, const VEC4& color, const VEC2& charSize);

	GUI::CFont& getFont(int size);

	void showDialog(const std::string& text, const int& fontSize = 16, bool cancelable = true);
	void hideDialog();
	void setDialogActive(bool active);
	bool isDialogActive();
	void setCancelableWithButton(bool cancelable);

	VEC2 getResolution();

private:
	VEC2 resolution = { 1920, 1080 };
	CCameraGui _orthoCamera;
	const CRenderTechnique* _technique = nullptr;
	const CRenderTechnique* _technique_clamp = nullptr;
	const CRenderTechnique* _technique_mask = nullptr;
	const CRenderTechnique* _technique_font = nullptr;
	const CRenderMesh* _quadMesh = nullptr;
	const CTexture* _fontTexture = nullptr;
	const CTexture* _nullMaskTexture = nullptr;

	GUI::VWidgets _registeredWidgets;
	GUI::VWidgets _activeWidgets;
	MVariants _variables;
	GUI::VControllers _controllers;

	std::string fontsFile = "data/gui/fonts.json";
	std::map<int, GUI::CFont> fonts;

	bool dialogActive = true;
	GUI::CDialogController* dialogController = nullptr;
};
