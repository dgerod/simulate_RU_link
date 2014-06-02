// =============================================================================
//
// =============================================================================
#ifndef MODEL_POLY3PROFILE_H
#define MODEL_POLY3PROFILE_H

class Poly3Profile
{
public:

    struct Data { double pos, vel; };

    bool initialize (const Data Initial, const Data Final, const double TotalTimeInSec);
    bool calcPosition (const double ElapsedTimeInSec, double* Pos);

private:

    bool calcCoeficients (const Data* Initial, const Data* Final,
                             const double TimeNs);

    struct Coeficients { double a0, a1, a2, a3; };
    struct Data _initial, _final;
    struct Coeficients _coeficients;
    double _timeInSec;
};

#endif
// =============================================================================
