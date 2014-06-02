// =============================================================================
//
// =============================================================================

#include <math.h>
#include <GL/glut.h>
#include "../helpers.h"
#include "common.h"
#include "Model.h"

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
    double pose[6];
    KDL::Frame frame;

    double q[5];

    q[0] = 0.0;
    q[1] = Joints(0);
    q[2] = Joints(1);
    q[3] = Joints(2);
    q[4] = 0.0;

    unsigned int nl = Model._chain.getNrOfSegments();
    for( unsigned int ldx=0; ldx<nl; ldx++)
    {
        double qi = q[ldx];
        std::cout << qi << std::endl;
        frame = frame * Model._chain.getSegment(ldx).pose( qi );

        frame_to_opengl(frame, pose);
        _objects[ldx]->setPose(pose);

        printf("p(0) = %f, p(1) = %f, p(2) = %f, p(3) = %f, p(4) = %f, p(5) = %f\n",
                pose[0], pose[1], pose[2], pose[3], pose[4], pose[5]);
    }
}

// -----------------------------------------------------------------------------
void
Model::drawIt ()
{
    std::cout << "[Model::drawIt] start" << std::endl;
    std::cout << "Num frames: " <<  _objects.size() << std::endl;

    // Draw reference frames

    for(ObjectList::iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
        (*it)->drawIt();glPopMatrix ();
    }

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
}

// -----------------------------------------------------------------------------
}

// =============================================================================
