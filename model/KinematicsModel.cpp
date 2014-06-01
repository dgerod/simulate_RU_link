// =============================================================================
//
// =============================================================================

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "KinematicsModel.h"

using namespace KDL;

// -----------------------------------------------------------------------------
KinematicsModel::KinematicsModel ()
{

}

// -----------------------------------------------------------------------------
bool
KinematicsModel::initialize ()
{
    std::cout << "[KinematicsModel::initialize] begin" << std::endl;

    bool Success = false;

    // DH parameters: [a, alpha, d, theta]
    // Origin
    _chain.addSegment( Segment(Joint(Joint::None),Frame::DH(1/*10*/, 0, 0, 0) ));
    // Revolution joint (1 dof)
    _chain.addSegment( Segment(Joint(Joint::RotZ),Frame::DH(2/*20*/, 0, 0, 0) ));
    /// Universal joint (2 dof)
    _chain.addSegment( Segment(Joint(Joint::RotZ),Frame::DH(0, 0, 0, 0) ));
    _chain.addSegment( Segment(Joint(Joint::RotZ),Frame::DH(1/*100*/, -M_PI/2, 0, 0) ));


    _joints.resize(_chain.getNrOfJoints());
    KDL::SetToZero(_joints);

    std::cout << "[KinematicsModel::initialize] end" << std::endl;
    return Success;
}

// -----------------------------------------------------------------------------
void
KinematicsModel::jntsToCart (const KDL::JntArray& Joints, KDL::Frame& Pose)
{
    ChainFkSolverPos_recursive fkSolver(_chain);
    fkSolver.JntToCart(Joints, Pose);
}

// =============================================================================
