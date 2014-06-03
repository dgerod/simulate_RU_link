// =============================================================================
//
// =============================================================================

#include <math.h>
#include <QDebug>
#include <GL/glut.h>
#include "../helpers.h"
#include "common.h"
#include "Model.h"

// -----------------------------------------------------------------------------
namespace gfxl {

// -----------------------------------------------------------------------------
Joint::Joint ()
  : Object()
{
  _length = 1.0;
}
// -----------------------------------------------------------------------------

void
Joint::drawIt ()
{
    qDebug( "[RefFrame::drawIt] begin" );

    float origin[3] = {0,0,0};
    float xp[3] = {_length,0,0}, yp[3] = {0,_length,0}, zp[3] = {0,0,_length};

    glPushMatrix ();

    // Tr = t*[ (Rx * (Ry*Rz) ]
    glTranslatef (_pose[0], _pose[1], _pose[2]);
    glRotatef (_pose[5] , 0,0,1);
    glRotatef (_pose[4], 0,1,0);
    glRotatef (_pose[3] , 1,0,0);

    glScalef (0.25, 0.25, 0.25);
    glLineWidth (2.0);

    glBegin (GL_LINES);
    glColor3f (1,0,0); // X axis is red.
    glVertex3fv (origin);
    glVertex3fv (xp);
    glColor3f (0,1,0); // Y axis is green.
    glVertex3fv (origin);
    glVertex3fv (yp);
    glColor3f (0,0,1); // z axis is blue.
    glVertex3fv (origin);
    glVertex3fv (zp);
    glEnd();

    glPopMatrix ();

    qDebug( "[Joint::drawIt] end" );
}

// -----------------------------------------------------------------------------
JointR::JointR ()
    : Joint()
{
}

// -----------------------------------------------------------------------------
void
JointR::drawIt ()
{
  // Draw cylinder
  glPushMatrix();
  glTranslatef(_pose[0], _pose[1], _pose[2]);
  glRotatef(_pose[5], 0,0,1);
  glRotatef(_pose[4], 0,1,0);
  glRotatef(_pose[3], 1,0,0);
  glColor3f(0.8,0.6,0.0);
  gluCylinder(gluNewQuadric(), 0.1, 0.1, 0.1, 10, 25);
  glPopMatrix();

  // Draw frame of the joint
  Joint::drawIt();
}
// -----------------------------------------------------------------------------
JointU::JointU ()
    : Joint()
{
}

// -----------------------------------------------------------------------------
void
JointU::drawIt ()
{
  // Draw sphere
  glPushMatrix();
  glTranslatef(_pose[0], _pose[1], _pose[2]);
  glRotatef(_pose[5], 0,0,1);
  glRotatef(_pose[4], 0,1,0);
  glRotatef(_pose[3], 1,0,0);
  glColor3f(0.8,0.6,0.0);
  gluSphere(gluNewQuadric(), 0.1, 10, 25);
  glPopMatrix();

  // Draw frame of the joint
  Joint::drawIt();
}

// -----------------------------------------------------------------------------
Model::Model ()
{
    _objects.clear();
}

// -----------------------------------------------------------------------------
void
Model::initialize (const KinematicsModel& KinemModel)
{
    qDebug( "[Model::initialize] start" );

    double pose[6];
    KDL::Frame frame;

    _objects.push_back(new JointR());
    _objects.push_back(new JointR());
    _objects.push_back(new JointU());
    _objects.push_back(new JointU());
    _objects.push_back(new RefFrame());

    unsigned int nl = KinemModel._chain.getNrOfSegments();
    for( unsigned int ldx=0; ldx<nl; ldx++)
    {
        double q = 0.0;
        frame = frame * KinemModel._chain.getSegment(ldx).pose( q );

        frame_to_opengl(frame, pose);
        _objects[ldx]->setPose(pose);
    }

    qDebug( "[Model::initialize] end" );
}

// -----------------------------------------------------------------------------
void
Model::finalize ()
{
    qDebug( "[Model::finalize] start" );

    for(ObjectList::iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
        delete (*it);
    }
    _objects.clear();

    qDebug( "[Model::finalize] end" );
}

// -----------------------------------------------------------------------------
void
Model::update (const KinematicsModel& Model, const KDL::JntArray& Joints)
{
    qDebug( "[Model::update] start" );

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
        frame = frame * Model._chain.getSegment(ldx).pose( qi );

        frame_to_opengl(frame, pose);
        _objects[ldx]->setPose(pose);

        /*
        printf("p(0) = %f, p(1) = %f, p(2) = %f, p(3) = %f, p(4) = %f, p(5) = %f\n",
                pose[0], pose[1], pose[2], pose[3], pose[4], pose[5]);
        */
    }

    qDebug( "[Model::update] end" );
}

// -----------------------------------------------------------------------------
void
Model::drawIt ()
{
    qDebug( "[Model::drawIt] start" );
    std::cout << "Num frames: " <<  _objects.size() << std::endl;

    for(ObjectList::iterator it = _objects.begin(); it != _objects.end(); ++it)
    {
        (*it)->drawIt();
    }

    qDebug( "[Model::drawIt] end" );
}

// -----------------------------------------------------------------------------
}

// =============================================================================
