// =============================================================================
//
// =============================================================================
#ifndef MODEL_KINEMATICSMODEL_H
#define MODEL_KINEMATICSMODEL_H

#include <stdio.h>
#include <kdl/chain.hpp>
#include <kdl/chainfksolver.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/frames_io.hpp>

class KinematicsModel
{
public:
    KinematicsModel();
    bool initialize ();
    void jntsToCart (const KDL::JntArray& Joints, KDL::Frame& Pose);

public:
    KDL::JntArray _joints;
    KDL::Frame _tcs0;
    KDL::Chain _chain;
};

#endif
// =============================================================================

