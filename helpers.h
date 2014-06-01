//=============================================================================
//
//=============================================================================
#ifndef APP_HELPERS_H__
#define APP_HELPERS_H__

#include <QString>
#include <kdl/frames.hpp>
#include <kdl/jntarray.hpp>

void
frame_to_QString (KDL::Frame& Frame, QString& Str);

void
joints_to_QString (KDL::JntArray& Joints, QString& Str);

void
frame_to_opengl (KDL::Frame& Frame, double Pose[6]);

#endif
//=============================================================================
