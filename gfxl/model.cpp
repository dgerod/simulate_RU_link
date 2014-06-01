// =============================================================================
//
// =============================================================================

#include <math.h>
#include <GL/glut.h>
#include "../helpers.h"
#include "common.h"
#include "model.h"

// -----------------------------------------------------------------------------
namespace gfxl {

// -----------------------------------------------------------------------------
Model::Model ()
{
    _objects.clear();
}

// -----------------------------------------------------------------------------
void
Model::initialize (const KinematicsModel& Model)
{
    std::cout << "[Model::initialize] begin" << std::endl;

    double pose[6];
    KDL::Frame frame;

    unsigned int nl = Model._chain.getNrOfSegments();
    for( unsigned int ldx=0; ldx<nl; ldx++)
    {
        double q = 0.0;
        frame = frame * Model._chain.getSegment(ldx).pose( q );

        //pose[0] = frame.p[0]; pose[1] = frame.p[1]; pose[2] = frame.p[2];
        //frame.M.GetEulerZYX(pose[5], pose[4], pose[3]);

        frame_to_opengl(frame, pose);

        _objects.push_back(new RefFrame());
        _objects.back()->setPose(pose);
    }

   // std::cout << "[Model::initialize] end" << std::endl;
}

// -----------------------------------------------------------------------------
void
Model::finalize ()
{
    for(ObjectList::iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
        delete (*it);
    }

    _objects.clear();
}

// -----------------------------------------------------------------------------
void
Model::update (const KinematicsModel& Model, const KDL::JntArray& Joints)
{
    //std::cout << "[Model::update] begin" << std::endl;

    double pose[6];
    KDL::Frame frame;

    unsigned int nl = Model._chain.getNrOfSegments();
    for( unsigned int ldx=0; ldx<nl; ldx++)
    {
        double q = Joints(ldx);
        frame = frame * Model._chain.getSegment(ldx).pose( q );

        //pose[0] = frame.p[0]; pose[1] = frame.p[1]; pose[2] = frame.p[2];
        //frame.M.GetEulerZYX(pose[5], pose[4], pose[3]);

        frame_to_opengl(frame, pose);

        _objects.back()->setPose(pose);
    }

    //std::cout << "[Model::initialize] end" << std::endl;
}


// -----------------------------------------------------------------------------
void
Model::drawIt ()
{
    std::cout << "[Model::drawIt] start" << std::endl;
    std::cout << "Num frames: " <<  _objects.size() << std::endl;

    for(ObjectList::iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
        (*it)->drawIt();glPopMatrix ();
    }

    // ------------------------------

    // Draw cylinder
    glPushMatrix ();
    glTranslatef (-2, -2, 0);

    glColor3f(0.8,0.6,0.0);
    gluCylinder(gluNewQuadric(), 0.5, 0.5, 3.0, 10, 25);
    glPopMatrix ();

    // Draw sphere
    glPushMatrix ();
    glTranslatef (-2, 2, 0);

    gluSphere(gluNewQuadric(), 0.5, 10, 25);
    glPopMatrix ();

    //std::cout << "[Model::drawIt] end" << std::endl;
}

// -----------------------------------------------------------------------------
}

// =============================================================================
