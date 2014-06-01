// =============================================================================
//
// =============================================================================
#ifndef GFXL_GRAPHICS_H
#define GFXL_GRAPHICS_H

#include <iostream>
#include <vector>
#include <kdl/frames.hpp>
#include "common.h"

namespace gfx {

// -----------------------------------------------------------------------------
class Object
{
public:

    Object ();
    virtual void drawIt () = 0;

    void setPosition (double X, double Y, double Z);
    void setPose (double Pose[6]);
    void setPose (KDL::Frame Frame) {};

    //
    // -----------------------------------

    double _pose[6];

};

// -----------------------------------------------------------------------------
class RefFrame : public Object
{
public:
    void drawIt();

};

// -----------------------------------------------------------------------------
class Floor : public Object
{
public:

    void drawIt ();
};

// -----------------------------------------------------------------------------
}

#endif
// =============================================================================

