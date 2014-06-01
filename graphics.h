//=============================================================================
//
//=============================================================================
#ifndef APP_GRAPHICS_H__
#define APP_GRAPHICS_H__

#include "gfxl/base.h"
#include "gfxl/model.h"

using namespace gfxl;

class Graphics
{
public:
  RefFrame _origin;
  Floor _floor;
  Model _model;
};

#endif
