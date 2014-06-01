// =============================================================================
//
// =============================================================================
#ifndef MODEL_KINEMATICSMODEL_H
#define MODEL_KINEMATICSMODEL_H

#include <stdio.h>
#include <kdl/chain.hpp>
#include <kdl/frames_io.hpp>

class KinematicsModel
{
public:
    KinematicsModel();
    bool initialize (double L1, double L2, double L3);

    bool jntsToCart (const KDL::JntArray& Joints, KDL::Frame& Pose);
    bool cartTojnts (const KDL::Frame& Pose, KDL::JntArray& Joints);

public:
    KDL::JntArray _joints;
    KDL::Frame _tcs0;
    KDL::Chain _chain;
};

#endif
// =============================================================================

