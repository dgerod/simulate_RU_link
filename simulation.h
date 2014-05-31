//=============================================================================
//
//=============================================================================
#ifndef APP_SIMULATION_H__
#define APP_SIMULATION_H__

#include "KinematicsModel.h"
#include "Movement.h"

class Simulation
{
public:
  KinematicsModel _model;
  Poly3Profile _trajectory;
};

#endif
//==============================================================================

