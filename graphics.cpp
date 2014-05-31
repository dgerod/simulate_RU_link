// =============================================================================
//
// =============================================================================

#include <math.h>
#include <GL/glut.h>
#include "graphics.h"

#define RAD_TO_DEG(x)   (180/M_PI)*x

// -----------------------------------------------------------------------------
namespace gfx {

// -----------------------------------------------------------------------------
Object::Object ()
{
  _pose[0] = _pose[1] = _pose[2] = 0.0f;
  _pose[3] = _pose[4] = _pose[5] = 0.0f;
}
// -----------------------------------------------------------------------------
void
Object::setPosition (double X, double Y, double Z)
{
  _pose[0] = X; _pose[1] = Y; _pose[2] = Z;
  _pose[3] = _pose[4] = _pose[5] = 0.0f;
}
// -----------------------------------------------------------------------------
void
Object::setPose (double Pose[6])
{
  _pose[0] = Pose[0]; _pose[1] = Pose[1]; _pose[2] = Pose[2];
  _pose[3] = RAD_TO_DEG(Pose[3]); _pose[4] = RAD_TO_DEG(Pose[4]); _pose[5] = RAD_TO_DEG(Pose[5]);
}

// -----------------------------------------------------------------------------
void
Floor::drawIt ()
{
    //std::cout << "[Floor::drawIt] end" << std::endl;

    double minX, maxX, minY, maxY;

    minX = -2; maxX = 2;
    minY = -2; maxY = 2;

    glColor3f(.3,.3,.3);
    glBegin (GL_QUADS);
        glVertex3f ( minX, maxY, -0.001);
        glVertex3f ( minX, minY, -0.001);
        glVertex3f ( maxX, minY, -0.001);
        glVertex3f ( maxX, maxY, -0.001);
    glEnd ();

    int num = 10;

    /*
    glBegin(GL_LINES);
    for(int i=0; i<=num; i++)
    {
        if (i==0) { glColor3f(.6,.3,.3); } else { glColor3f(.25,.25,.25); };
        glVertex3f(i,0,0);
        glVertex3f(i,maxY,0);
        if (i==0) { glColor3f(.3,.3,.6); } else { glColor3f(.25,.25,.25); };
        glVertex3f(0,i,0);
        glVertex3f(maxX,i,0);
    };
    glEnd();tx = _pose[0]; ty = _pose[1]; tz = _pose[2];
    */

    //std::cout << "[Floor::drawIt] end" << std::endl;
}

// -----------------------------------------------------------------------------
void
RefFrame::drawIt ()
{
    std::cout << "[RefFrame::drawIt] begin" << std::endl;


    for (int idx=0; idx<6; idx++)
      { std::cout << _pose[idx] << " ";}
    std::cout << std::endl;

    float origin[3] = {0,0,0};
    float xp[3] = {1,0,0}, yp[3] = {0,1,0}, zp[3] = {0,0,1};

    float tx, ty, tz;
    float rx, ry, rz;

    tx = _pose[0]; ty = _pose[1]; tz = _pose[2];
    rx = _pose[3]; ry = _pose[4]; rz = _pose[5];

    glPushMatrix ();

    glTranslatef (tx, ty, tz);
    glRotatef (rz , 0,0,1);
    glRotatef (ry, 0,1,0);
    glRotatef (rx , 1,0,0);

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

   std::cout << "[RefFrame::drawIt] end" << std::endl;
}

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

        pose[0] = frame.p[0]; pose[1] = frame.p[1]; pose[2] = frame.p[2];
        frame.M.GetEulerZYX(pose[5], pose[4], pose[3]);

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

        pose[0] = frame.p[0]; pose[1] = frame.p[1]; pose[2] = frame.p[2];
        frame.M.GetEulerZYX(pose[5], pose[4], pose[3]);

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
    glRotatef (0, 1,0,0);

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
