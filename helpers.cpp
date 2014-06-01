
//=============================================================================
//
//=============================================================================

#include "helpers.h"

// ----------------------------------------------------------------------------
void
frame_to_QString (KDL::Frame& Frame, QString& Str)
{
    double x, y, z;
    double alpha, beta, gamma;

    x = Frame.p(0); y = Frame.p(1); z = Frame.p(2);
    Frame.M.GetEulerZYX(alpha, beta, gamma);

    Str = QString("[%1, %2, %3, %4, %5, %6]")
             .arg(QString::number(x),QString::number(y),QString::number(z),
                  QString::number(gamma),QString::number(beta),QString::number(alpha));
}

// ----------------------------------------------------------------------------
void
frame_to_opengl (KDL::Frame& Frame, double Pose[6])
{
    double x, y, z;
    double alpha, beta, gamma;

    x = Frame.p(0); y = Frame.p(1); z = Frame.p(2);
    Frame.M.GetEulerZYX(alpha, beta, gamma);

    Pose[0] = x; Pose[1] = y; Pose[2] = z;
    Pose[3] = gamma; Pose[4] = beta; Pose[5] = alpha;

}
//=============================================================================