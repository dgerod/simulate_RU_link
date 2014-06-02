// =============================================================================
//
// =============================================================================
#ifndef MODEL_MOVEMENT_H
#define MODEL_MOVEMENT_H

#include <vector>
#include "Poly3Profile.h"

class Movement
{
public:

    Movement ();
    void initialize (unsigned int NumJoints);
    void finalize ();

    bool start (KDL::JntArray& Qi, KDL::JntArray& Qf, double Tt);
    bool nextPosition (double Tk, KDL::JntArray& Qk);

    //
    // -----------------------------------

    double _Tk, _Tt;

    unsigned int _numJoints;
    typedef std::vector<Poly3Profile*> tProfileList;
    tProfileList _profiles;


};

#endif
// =============================================================================
