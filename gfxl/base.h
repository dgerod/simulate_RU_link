// =============================================================================
//
// =============================================================================
#ifndef GFXL_BASE_H
#define GFXL_BASE_H

#include <iostream>
#include <vector>
#include <kdl/frames.hpp>

namespace gfxl {

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

    RefFrame ();
    void drawIt();

    //
    // -----------------------------------

    double _length;
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

