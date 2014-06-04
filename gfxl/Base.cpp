// =============================================================================
//
// =============================================================================

#include <math.h>
#include <QDebug>
#include <GL/glut.h>
#include "common.h"
#include "Base.h"

// -----------------------------------------------------------------------------
namespace gfxl {

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
    qDebug( "[Floor::drawIt] end" );
    double minX, maxX, minY, maxY;

    // Draw solid

    minX = -20; maxX = 20;
    minY = -20; maxY = 20;

    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_QUADS);
    glVertex3f( minX, maxY, -0.2);
    glVertex3f( minX, minY, -0.2);
    glVertex3f( maxX, minY, -0.2);
    glVertex3f( maxX, maxY, -0.2);
    glEnd();

    // Draw grid

    glPushMatrix();
    glTranslatef( -100/2, -100/2, -0.1);

    glBegin(GL_LINES);
    glColor3f(0.5, 0.5, 0.5);
    for(int i = 0; i <= 100; i += 10)
    {
        glVertex3f((float)i, 0.0f, 0.0f);
        glVertex3f((float)i, 100.0f, 0.0f);
        glVertex3f(0.0f, (float)i, 0.0f);
        glVertex3f(100.0f, (float)i, 0.0f);
    }
    glEnd();

    glPopMatrix();

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
    glEnd();
    */

    qDebug( "[Floor::drawIt] end" );
}

// -----------------------------------------------------------------------------
RefFrame::RefFrame ()
    : Object()
{
    _length = 100.0;;
}

// -----------------------------------------------------------------------------
RefFrame::RefFrame (double L)
    : Object()
{
    _length = L;
}

// -----------------------------------------------------------------------------
void
RefFrame::drawIt ()
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
    glColor3f (1,0,0);
    glVertex3fv (origin);
    glVertex3fv (xp);
    glColor3f (0,1,0);
    glVertex3fv (origin);
    glVertex3fv (yp);
    glColor3f (0,0,1);
    glVertex3fv (origin);
    glVertex3fv (zp);
    glEnd();

    glPopMatrix ();

    qDebug( "[RefFrame::drawIt] end" );
}

// -----------------------------------------------------------------------------
}

// =============================================================================
