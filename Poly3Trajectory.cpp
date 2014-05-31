// =============================================================================
//
// =============================================================================
#include <stdio.h>
#include <stddef.h>
#include <math.h>

#include "Poly3Trajectory.h"

// -----------------------------------------------------------------------------
bool
Poly3Profile::initialize (const Poly3Profile::Data Initial, const Data Final,
		             const double TotalTimeInSec)
{
    bool Success = false;

    _initial = Initial;
    _final = Final;
    _timeInSec = TotalTimeInSec;
    Success = calcCoeficients ( &_initial, &_final, _timeInSec );

    return Success;
}

// -----------------------------------------------------------------------------
bool
Poly3Profile::calcPosition (const double ElapsedTimeInSec, double* Pos)
{
    bool Success = false;
    double timeSec = ElapsedTimeInSec;

    /*
    RCLOG_TRACE( cRCLogCatProf, "[Poly3Profile] ::Poly3Profile_calcPos()")
    */

    // Check pointers
    if( NULL == Pos )
    {
            return Success;
    }

    *Pos = _coeficients.a0 + timeSec *(_coeficients.a1 + timeSec *
                            (_coeficients.a2 + (timeSec * _coeficients.a3)) );

    //vel = this->_coeficients.a1 + timeSec *
    //		( (2.0 * this->_coeficients.a2)  + (timeSec * (3.0 * this->_coeficients.a3) ));

    Success = true;
    return Success;
}

// -----------------------------------------------------------------------------
bool
Poly3Profile::calcCoeficients (const Poly3Profile::Data* Initial, const Poly3Profile::Data* Final,
				   const double TotalTimeInSec)
{
    bool Success = false;

    double timeSec = TotalTimeInSec;
    double timeSecBy2 = timeSec * timeSec;
    double h;

    // Check pointers
    if(NULL == Initial || NULL == Final)
    {
            return Success;
    }

    h = Final->pos -Initial->pos;
    _coeficients.a0 = Initial->pos;
    _coeficients.a1 = Initial->vel;
    _coeficients.a2 = ((3.0*h -((2.0*Initial->vel+Final->vel)*timeSec))/(timeSecBy2));
    _coeficients.a3 = ((-2.0*h +((Initial->vel+Final->vel)*timeSec))/(timeSecBy2*timeSec));

    /*
    RCLOG_TRACE( cRCLogCatProf,"Poly3Profile_getCoef: a0:%f a1:%f a2:%f a3:%f",
                         this->_coeficients.a0,this->_coeficients.a1,this->_coeficients.a2,this->_coeficients.a3);
    */

    Success = true;
    return  Success;
}

// =============================================================================
