// =============================================================================
//
// =============================================================================

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <kdl/chainfksolver.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainiksolverpos_lma.hpp>
#include "KinematicsModel.h"

using namespace KDL;

// -----------------------------------------------------------------------------
KinematicsModel::KinematicsModel ()
{
}

// -----------------------------------------------------------------------------
bool
KinematicsModel::initialize (double L1, double L2, double L3)
{
    std::cout << "[KinematicsModel::initialize] begin" << std::endl;

    bool Success = false;

    // Create the kinematics model of RU link
    // DH parameters: [a, alpha, d, theta]

    // Origin
    _chain.addSegment( Segment( Joint(Joint::None),Frame::DH(L1, 0, 0, 0) ));
    // Revolution joint (1 dof)
    _chain.addSegment( Segment( Joint(Joint::RotZ),Frame::DH(L2, 0, 0, 0) ));
    /// Universal joint (2 dof)
    _chain.addSegment( Segment( Joint(Joint::RotZ),Frame::DH(0, 0, 0, 0) ));
    _chain.addSegment( Segment( Joint(Joint::RotZ),Frame::DH(0, M_PI/2, 0, 0) ));
    _chain.addSegment( Segment( Joint(Joint::None),Frame::DH(L3, -M_PI/2, 0, 0) ));

    std::cout << "num joints " << _chain.getNrOfJoints() << std::endl;

    // Initialize joints to zero

    _joints.resize(_chain.getNrOfJoints());
    KDL::SetToZero(_joints);

    std::cout << "[KinematicsModel::initialize] end" << std::endl;
    return Success;
}

// -----------------------------------------------------------------------------
bool
KinematicsModel::jntsToCart (const KDL::JntArray& Joints, KDL::Frame& Pose)
{
    bool Success = false;

    ChainFkSolverPos_recursive fkSolver(_chain);
    if( fkSolver.JntToCart(Joints, Pose) >= 0 )
    {
        Success = true;
    }

    return Success;
}

// -----------------------------------------------------------------------------
bool
KinematicsModel::cartTojnts (const KDL::Frame& Pose, KDL::JntArray& Joints)
{
    bool Success = false;

    ChainIkSolverPos_LMA ikSolver(_chain);
    if( ikSolver.CartToJnt(_joints, Pose, Joints) >= 0 )
    {
        for(unsigned int jdx=0; jdx< Joints.rows(); jdx++)
        {
            if( Joints(jdx) > M_PI )
            {
                while( Joints(jdx) > M_PI )
                  { Joints(jdx) = Joints(jdx) - 2*M_PI; }
            }
            else if( Joints(jdx) < -M_PI )
            {
                Joints(jdx) = Joints(jdx) + 2*M_PI;
            }
        }

        Success = true;
    }

    return Success;
}

// =============================================================================
