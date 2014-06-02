
// =============================================================================
//
// =============================================================================

#include <iostream>
#include <QtGlobal>
#include <QtGui>
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
joints_to_QString (KDL::JntArray& Joints, QString& Str)
{
    double j1, j2, j3;

    j1 = Joints(0); j2 = Joints(1); j3 = Joints(2);

    Str = QString("[%1, %2, %3,]")
             .arg(QString::number(j1),QString::number(j2),QString::number(j3));
}

// ----------------------------------------------------------------------------
void
frame_to_opengl (KDL::Frame& Frame, double Pose[])
{
    double x, y, z;
    double alpha, beta, gamma;

    x = Frame.p(0); y = Frame.p(1); z = Frame.p(2);
    Frame.M.GetEulerZYX(alpha, beta, gamma);

    Pose[0] = x; Pose[1] = y; Pose[2] = z;
    Pose[3] = gamma; Pose[4] = beta; Pose[5] = alpha;
}

// ----------------------------------------------------------------------------
bool
proc_position_input (QString& Input, int NumElements, double Elements[6])
{
    QRegExp rx("[, ]");
    QStringList list = Input.split(rx, QString::SkipEmptyParts);

    if( list.length() != NumElements )
    {
        std::cout << "ERROR: Incorrect input = " << Input.toStdString() << std::endl;
        return false;
    }

    for(unsigned int idx=0; idx<list.length(); idx++)
    {
        bool validate;
        Elements[idx] = list[idx].toDouble(&validate);

        if( validate == false )
        {
            return false;
        }
    }

    return true;
}

// =============================================================================
