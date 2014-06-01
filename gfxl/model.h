// =============================================================================
//
// =============================================================================
#ifndef GFXL_MODEL_H
#define GFXL_MODEL_H

#include <iostream>
#include <vector>
#include <kdl/frames.hpp>

#include "common.h"
#include "base.h"
#include "../simulation.h"

namespace gfx {

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
