#pragma once


class TCompRenderUI : public TCompBase {
  DECL_SIBLING_ACCESS();

private:

  std::vector<std::function<void()>> toRender = std::vector<std::function<void()>>();

public:
  //void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void renderUI();
  void registerOnRenderUI(std::function<void()> renderUi);
  
};
