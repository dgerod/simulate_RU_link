// =============================================================================
//
// =============================================================================
#include <stdio.h>
#include <iostream>
#include <stddef.h>
#include <math.h>
#include <QDebug>
#include "Poly3Profile.h"

// -----------------------------------------------------------------------------
bool
Poly3Profile::initialize (const Poly3Profile::Data Initial, const Data Final,
		             const double TotalTimeInSec)
{
    qDebug( "[Poly3Profile::initialize] start" );
    bool Success = false;

    _initial = Initial;
    _final = Final;
    _timeInSec = TotalTimeInSec;
    Success = calcCoeficients ( &_initial, &_final, _timeInSec );

    qDebug( "[Poly3Profile::initialize] end" );
    return Success;
}

// -----------------------------------------------------------------------------
bool
Poly3Profile::calcPosition (const double ElapsedTimeInSec, double* Pos)
{
    qDebug( "[Poly3Profile::calcPosition] start" );
    bool Success = true;
    double timeSec = ElapsedTimeInSec;

    *Pos = _coeficients.a0 + timeSec *(_coeficients.a1 + timeSec *
                            (_coeficients.a2 + (timeSec * _coeficients.a3)) );

    qDebug( "[Poly3Profile::calcPosition] end" );
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

    h = Final->pos -Initial->pos;
    _coeficients.a0 = Initial->pos;
    _coeficients.a1 = Initial->vel;
    _coeficients.a2 = ((3.0*h -((2.0*Initial->vel+Final->vel)*timeSec))/(timeSecBy2));
    _coeficients.a3 = ((-2.0*h +((Initial->vel+Final->vel)*timeSec))/(timeSecBy2*timeSec));

    Success = true;
    return  Success;
}

// =============================================================================
