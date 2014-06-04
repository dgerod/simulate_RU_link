// =============================================================================
//
// =============================================================================
#ifndef GFXL_MODEL_H
#define GFXL_MODEL_H

#include <iostream>
#include <vector>
#include <kdl/frames.hpp>
#include "Base.h"
#include "../model/KinematicsModel.h"

namespace gfxl {

// -----------------------------------------------------------------------------
class Joint : public Object
{
public:

    Joint ();
    void drawIt();

    double _length;
};

// -----------------------------------------------------------------------------
class JointR : public Joint
{
public:

    JointR ();
    JointR (double H, double R);
    void drawIt();

    double _height, _radius;
};

// -----------------------------------------------------------------------------
class JointU : public Joint
{
public:

    JointU ();
    JointU (double R);
    void drawIt();

    double _radius;
};

// -----------------------------------------------------------------------------
class Model : public Object
{
public:

    Model ();
    void initialize (const KinematicsModel& KinemModel);
    void finalize ();
    void update (const KinematicsModel& Chain, const KDL::JntArray& Joints);
    void drawIt ();

protected:

    typedef std::vector<Object*> ObjectList;
    ObjectList _objects;
};
// -----------------------------------------------------------------------------
}

#endif
// =============================================================================

