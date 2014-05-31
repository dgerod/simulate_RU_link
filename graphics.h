// =============================================================================
//
// =============================================================================
#ifndef APP_GRAPHICS_H
#define APP_GRAPHICS_H

#include <iostream>
#include <vector>
#include <kdl/frames.hpp>
#include "simulation.h"

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
class Model : public Object
{
public:
    Model ();
    void initialize (const KinematicsModel& Chain);
    void finalize ();

    void update (const KinematicsModel& Chain, const KDL::JntArray& Joints);
    void drawIt ();

protected:

    //
    // -----------------------------------

    typedef std::vector<gfx::Object*> ObjectList;
    ObjectList _objects;
};
// -----------------------------------------------------------------------------
}

#endif
// =============================================================================

