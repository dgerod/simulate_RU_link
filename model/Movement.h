// =============================================================================
//
// =============================================================================
#ifndef MODEL_MOVEMENT_H
#define MODEL_MOVEMENT_H

#include <vector>
#include "../model/KinematicsModel.h"
#include "Poly3Profile.h"

class Movement
{
public:

    Movement ();
    void initialize (const KinematicsModel& KinemModel);
    void finalize ();
    bool start (KDL::JntArray& Qi, KDL::JntArray& Qf, double Tt);
    bool nextPosition (double Tk, KDL::JntArray& Qk);
    double Tk () { return _Tk; };

private:

    unsigned int _numJoints;
    typedef std::vector<Poly3Profile*> tProfileList;
    tProfileList _profiles;
    double _Tk, _Tt;
};

#endif
// =============================================================================
