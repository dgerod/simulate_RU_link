
// =============================================================================
//
// =============================================================================

#include <stdio.h>
#include <stddef.h>
#include <iostream>
#include <math.h>
#include <QDebug>
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
Movement::initialize (const KinematicsModel& KinemModel)
{
    _profiles.clear();
    _numJoints = KinemModel._chain.getNrOfJoints();

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
    qDebug( "[Movement::start] start" );
    bool Success = true;
    Poly3Profile::Data init, final;

    _Tk = 0;
    _Tt = Tt;

    for(unsigned int jdx=0; jdx<_numJoints; jdx++)
    {
        init.pos = Qi(jdx); init.vel = 0.0f;
        final.pos = Qf(jdx); final.vel = 0.0f;

        _profiles[jdx]->initialize(init, final, Tt);
    }

    qDebug( "[Movement::start] end" );
    return Success;
}

// -----------------------------------------------------------------------------
bool
Movement::nextPosition (double Tk, KDL::JntArray& Qk)
{
    qDebug( "[Movement::nextPosition] start" );
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

    qDebug( "[Movement::nextPosition] end" );
    return IsOngoing;
}

// =============================================================================
