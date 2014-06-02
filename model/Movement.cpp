
// =============================================================================
//
// =============================================================================

#include <stdio.h>
#include <stddef.h>
#include <iostream>
#include <math.h>
#include <kdl/jntarray.hpp>
#include "Movement.h"

using namespace KDL;

// -----------------------------------------------------------------------------
Movement::Movement ()
    : _numJoints(0)
{
  _profiles.clear();
}

// -----------------------------------------------------------------------------
void
Movement::initialize (unsigned int NumJoints)
{
    _profiles.clear();
    _numJoints = NumJoints;

    for(unsigned int jdx=0; jdx<_numJoints; jdx++)
    {
        _profiles.push_back( new Poly3Profile() );
    }
}

// -----------------------------------------------------------------------------
void
Movement::finalize ()
{
    for(tProfileList::iterator it = _profiles.begin(); it != _profiles.end(); ++it)
    {
        delete (*it);
    }

    _profiles.clear();
}

// -----------------------------------------------------------------------------
bool
Movement::start (KDL::JntArray& Qi, KDL::JntArray& Qf, double Tt)
{
    std::cout << "[Movement::start] begin" << std::endl;
    bool Success = true;

    _Tk = 0;
    _Tt = Tt;

    Poly3Profile::Data init, final;

    for(unsigned int jdx=0; jdx<_numJoints; jdx++)
    {
        init.pos = Qi(jdx); init.vel = 0.0f;
        final.pos = Qf(jdx); final.vel = 0.0f;

        _profiles[jdx]->initialize(init, final, Tt);
    }

    std::cout << "[Movement::start] end" << std::endl;
    return Success;
}

// -----------------------------------------------------------------------------
bool
Movement::nextPosition (double Tk, KDL::JntArray& Qk)
{
    std::cout << "[Movement::nextPosition] end" << std::endl;
    bool IsOngoing = true;
    double pos;

    for(unsigned int jdx=0; jdx<_numJoints; jdx++)
    {
        _profiles[jdx]->calcPosition(Tk, &pos);
         Qk(jdx) = pos;
    }

    _Tk = Tk;

    // Check if trajectory is finished
    if( _Tk >= _Tt)
    {
        IsOngoing = false;
    }

    std::cout << "[Movement::nextPosition] end" << std::endl;
    return IsOngoing;
}

// =============================================================================
