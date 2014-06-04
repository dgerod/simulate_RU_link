// =============================================================================
//
// =============================================================================

#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <QDebug>
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
KinematicsModel::initialize (double Pos[3], double L1, double L2)
{
    qDebug( "[KinematicsModel::initialize] start" );
    bool Success = false;

    // Define chain

    // Origin
    _chain.addSegment( Segment( Joint(Joint::None),Frame(Vector(Pos[0], Pos[1], Pos[2])) ));
    // Revolution joint (1 dof)
    _chain.addSegment( Segment( Joint(Joint::RotZ),Frame(Vector(L1, 0, 0)) ));
    /// Universal joint (2 dof)
    _chain.addSegment( Segment( Joint(Joint::RotZ),Frame(Vector(0, 0, 0)) ));
    _chain.addSegment( Segment( Joint(Joint::RotY),Frame(Vector(0, 0, 0)) ));
    // End-effector
    _chain.addSegment( Segment( Joint(Joint::None),Frame(Vector(L2, 0, 0)) ));

    // Initialize joints to zero

    _joints.resize(_chain.getNrOfJoints());
    KDL::SetToZero(_joints);

    qDebug( "[KinematicsModel::initialize] end" );
    return Success;
}

// -----------------------------------------------------------------------------
bool
KinematicsModel::jntsToCart (const KDL::JntArray& Joints, KDL::Frame& Pose)
{
    qDebug( "[KinematicsModel::jntsToCart] start" );
    bool Success = false;

    ChainFkSolverPos_recursive fkSolver(_chain);
    if( fkSolver.JntToCart(Joints, Pose) >= 0 )
    {
        Success = true;
    }

    qDebug( "[KinematicsModel::jntsToCart] end" );
    return Success;
}

// -----------------------------------------------------------------------------
bool
KinematicsModel::cartTojnts (const KDL::Frame& Pose, KDL::JntArray& Joints)
{
    qDebug( "[KinematicsModel::cartTojnts] start" );
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

    qDebug( "[KinematicsModel::cartTojnts] end" );
    return Success;
}

// =============================================================================
