// =============================================================================
//
// =============================================================================

#include <math.h>
#include <GL/glut.h>
#include "common.h"
#include "base.h"

#define RAD_TO_DEG(x)   (180/M_PI)*x

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

    glPushMatrix ();

    // Tr = t*[ (Rx * (Ry*Rz) ]

    glTranslatef (_pose[0], _pose[1], _pose[2]);
    glRotatef (_pose[3] , 0,0,1);
    glRotatef (_pose[4], 0,1,0);
    glRotatef (_pose[5] , 1,0,0);

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
}

// =============================================================================
