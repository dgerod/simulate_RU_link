// ============================================================================
//
// ============================================================================
#ifndef APP_GRAPHICS_H__
#define APP_GRAPHICS_H__

#include "gfxl/Base.h"
#include "gfxl/Model.h"

using namespace gfxl;

class Graphics
{
public:

    void draw ();

    Floor _floor;
    RefFrame _origin;
    Model _model;
};

#endif
// ============================================================================
