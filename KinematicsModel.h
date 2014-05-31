// =============================================================================
//
// =============================================================================
#ifndef __KinematicsModel_H__
#define __KinematicsModel_H__

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
    KDL::Chain _chain;
};

#endif
// =============================================================================

