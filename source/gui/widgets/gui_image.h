#pragma once

#include "gui/gui_widget.h"

namespace GUI
{
  class CImage : public CWidget
  {
  public:
    CImage() = default;
    void render() override;

  private:
    TImageParams _imageParams;

    friend class CParser;
  };
}