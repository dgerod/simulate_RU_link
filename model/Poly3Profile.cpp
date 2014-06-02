// =============================================================================
//
// =============================================================================
#include <stdio.h>
#include <iostream>
#include <stddef.h>
#include <math.h>

#include "Poly3Profile.h"

// -----------------------------------------------------------------------------
bool
Poly3Profile::initialize (const Poly3Profile::Data Initial, const Data Final,
		             const double TotalTimeInSec)
{
    std::cout << "[Poly3Profile::initialize] begin" << std::endl;
    bool Success = false;

    _initial = Initial;
    _final = Final;
    _timeInSec = TotalTimeInSec;
    Success = calcCoeficients ( &_initial, &_final, _timeInSec );

    std::cout << _coeficients.a0 << std::endl;
    std::cout << _coeficients.a1 << std::endl;
    std::cout << _coeficients.a2 << std::endl;
    std::cout << _coeficients.a3 << std::endl;

    std::cout << "[Poly3Profile::initialize] end" << std::endl;
    return Success;
}

// -----------------------------------------------------------------------------
bool
Poly3Profile::calcPosition (const double ElapsedTimeInSec, double* Pos)
{
    std::cout << "[Poly3Profile::calcPosition] begin" << std::endl;
    bool Success = false;
    double timeSec = ElapsedTimeInSec;

    *Pos = _coeficients.a0 + timeSec *(_coeficients.a1 + timeSec *
                            (_coeficients.a2 + (timeSec * _coeficients.a3)) );

    //vel = this->_coeficients.a1 + timeSec *
    //		( (2.0 * this->_coeficients.a2)  + (timeSec * (3.0 * this->_coeficients.a3) ));

    std::cout << "Position: " << *Pos << std::endl;
    std::cout << "[Poly3Profile::calcPosition] end" << std::endl;
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

    h = Final->pos -Initial->pos;
    _coeficients.a0 = Initial->pos;
    _coeficients.a1 = Initial->vel;
    _coeficients.a2 = ((3.0*h -((2.0*Initial->vel+Final->vel)*timeSec))/(timeSecBy2));
    _coeficients.a3 = ((-2.0*h +((Initial->vel+Final->vel)*timeSec))/(timeSecBy2*timeSec));

    Success = true;
    return  Success;
}

// =============================================================================
