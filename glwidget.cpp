// =============================================================================
//
// Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
//
// This file is part of the documentation of the Qt Toolkit.
//
// This file may be used under the terms of the GNU General Public
// License version 2.0 as published by the Free Software Foundation
// and appearing in the file LICENSE.GPL included in the packaging of
// this file.  Please review the following information to ensure GNU
// General Public Licensing requirements will be met:
// http://www.trolltech.com/products/qt/opensource.html
//
// If you are unsure which license is appropriate for your use, please
// review the following information:
// http://www.trolltech.com/products/qt/licensing.html or contact the
// sales department at sales@trolltech.com.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//https://www.google.es/search?client=ubuntu&channel=fs&q=qt+opengl+zoom&ie=utf-8&oe=utf-8&gfe_rd=cr&ei=vOSJU8-tEojA8gfpp4HAAQ
// =============================================================================

#include <math.h>
#include <QtGui>
#include <QtOpenGL>
#include <GL/glut.h>

#include "glwidget.h"

/* Function to build our font list */
void buildFont ( GLuint* base )
{
    /* Storage for 96 characters */
    *base = glGenLists( 96 );
}

// -----------------------------------------------------------------------------
GLWidget::GLWidget (QWidget* Parent)
	: QGLWidget(Parent)
{
    // Storage for 96 characters
    _base = glGenLists( 96 );

    // Camera
    _camPos[0] = 5; _camPos[1] = 5; _camPos[2] = 1;
    _xRot = 0; _yRot = 0; _zoom = 1;

    // Frame in the origin
    _origin.setPosition(0, 0, 0);

    // Load kinematics model and prepare graphics
    _simulation._model.initialize();
    _model.initialize( _simulation._model );
}

// -----------------------------------------------------------------------------
GLWidget::~GLWidget ()
{
    // Recover memory from list of characters
    glDeleteLists(_base, 96);
    makeCurrent();

    _model.finalize();
}

// -----------------------------------------------------------------------------
void
GLWidget::normalizeAngle (int* Angle)
{
    while (*Angle < 0)
    {   *Angle += 360 * 16; }
    while (*Angle > 360 * 16)
    { *Angle -= 360 * 16; }
}

// -----------------------------------------------------------------------------
QSize
GLWidget::minimumSizeHint () const
{
    return QSize(10, 10);
}

// -----------------------------------------------------------------------------
QSize
GLWidget::sizeHint () const
{
    return QSize(500, 500);
}

// -----------------------------------------------------------------------------
void
GLWidget::setXRotation (int Angle)
{
    normalizeAngle(&Angle);
    if (Angle != _xRot)
    {
        _xRot = Angle;
        emit xRotationChanged(Angle);
        updateGL();
    }
}

// -----------------------------------------------------------------------------
void
GLWidget::setYRotation (int Angle)
{
    normalizeAngle(&Angle);
    if (Angle != _yRot)
    {
        _yRot = Angle;
        emit yRotationChanged(Angle);
        updateGL();
    }
}
// -----------------------------------------------------------------------------
void
GLWidget::setZoom (int Increment)
{
    if (Increment != _zoom)
    {
        _zoom = Increment;
        emit zoomChanged(Increment);
        updateGL();
    }
}

// -----------------------------------------------------------------------------
void
GLWidget::initializeGL ()
{
    glClearColor(0.1, 0.1, 0.1, 0.0);	// sets background color to blue
    glClearDepth(1.0);			// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); 	//enables hidden-surface removal

    //glShadeModel(GL_FLAT);
    //glCullFace(GL_BACK);  //to display back face. We can put also GL_FRONT for front faces
    //glEnable(GL_CULL_FACE);	//enables displaying of both faces of polygons. (enables previous instruction)

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 1, 1, 100);
}

// -----------------------------------------------------------------------------
void
GLWidget::resizeGL (int Width, int Height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, Width/Height, 1, 100);
    glViewport(0, 0, Width, Height);
}

// -----------------------------------------------------------------------------
void
GLWidget::paintGL ()
{
    //std::cout << "--- [GLWidget::paintGL] --- " << std::endl;

    // Prepare the scene
    // ----------------------------------------------

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear Screen And Depth Buffer with previous values
    glMatrixMode(GL_MODELVIEW);	// Necessary to perform viewing and model transformations
    glLoadIdentity();		// Resets the model/view matrix

    // Set view
    // ----------------------------------------------

    gluLookAt(_camPos[0],_camPos[1],_camPos[2],0,0,0,0,0,1);
    // Rotate when user changes rotate_x and rotate_y
    glRotated(_xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(_yRot / 16.0, 0.0, 1.0, 0.0);
    // Zoom in and out according to mouse wheel movement
    glScalef(_zoom, _zoom, 1.0f);

    // On-screen information
    // ----------------------------------------------

    glColor3f(0.0f, 1.0f, 0.0f);
    glRasterPos2f(2.6f, -2.2f);
    displayText( "Hola" );

    // Draw scene
    // ----------------------------------------------

    //_origin.drawIt();
    _floor.drawIt();
    _model.drawIt();

    glFinish();
}

// -----------------------------------------------------------------------------
void
GLWidget::mousePressEvent (QMouseEvent* Event)
{
    _lastPos = Event->pos();
}

// -----------------------------------------------------------------------------
void
GLWidget::mouseMoveEvent (QMouseEvent* Event)
{
    int dx = Event->x() - _lastPos.x();
    int dy = Event->y() - _lastPos.y();

    if (Event->buttons() & Qt::LeftButton)
    {
        setXRotation(_xRot + 8 * dy);
    }
    else if (Event->buttons() & Qt::RightButton)
    {
        setYRotation(_yRot + 8 * dy);
    }

    _lastPos = Event->pos();
}

// -----------------------------------------------------------------------------
void
GLWidget::wheelEvent (QWheelEvent* Event)
{
    std::cout << "[GLWidget::wheelEvent]" << std::endl;

    QGLWidget::wheelEvent(Event);
    if (Event->isAccepted())
      return;

    //distance *= qPow(1.2, -Event->delta() / 120);
    //float distance = event->delta() / 8.0 / 15.0) * WHEEL_DELTA;
    //setZoom(distance);

    std::cout << Event->delta() << std::endl;

    Event->accept();
    update();
}

// -----------------------------------------------------------------------------
void GLWidget::keyPressEvent (QKeyEvent* Event)
{
  std::cout << "[GLWidget::keyPressEvent]" << std::endl;

  if(Event->key() == Qt::Key_A)
  {
  }
  else if(Event->key() == Qt::Key_Z)
  {
  }
  else
  {
      QWidget::keyPressEvent(Event);
  }
}

// -----------------------------------------------------------------------------

void
GLWidget::displayText( const char *fmt, ... )
{
    char text[256]; /* Holds our string */
    va_list ap;     /* Pointer to our list of elements */

    /* If there's no text, do nothing */
    if ( fmt == NULL )
        return;

    /* Parses The String For Variables */
    va_start( ap, fmt );
    vsprintf( text, fmt, ap );
    va_end( ap );

    /* Pushes the Display List Bits */
    glPushAttrib( GL_LIST_BIT );

    /* Sets base character to 32 */
    glListBase( _base - 32 );

    /* Draws the text */
    glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );

    /* Pops the Display List Bits */
    glPopAttrib();
}

// -----------------------------------------------------------------------------
void
GLWidget::moveByJoints ()
{
    std::cout << "[GLWidget::moveByJoints]" << std::endl;

    // ----------------------------------------------------

    int nj = _simulation._model._chain.getNrOfJoints();
    KDL::JntArray q(nj);
    KDL::Frame pose;

    QString str = _inputPose;

    QRegExp rx("[, ]");// match a comma or a space
    QStringList list = str.split(rx, QString::SkipEmptyParts);

    if( list.length() != nj )
    {
        std::cout << "ERROR: Incorrect input" << std::endl;
        return;
    }

    for(unsigned int idx=0; idx<list.length(); idx++)
    {
        q(idx) = list[0].toDouble();
    }

    // ----------------------------------------------------

    //q(0) = 0;
    //q(1) = 0;
    //q(2) = M_PI/4;

    printf("q(0)=%f,q(1)=%f,q(2)=%f\n", q(0), q(1), q(2));
    _simulation._model.jntsToCart(q, pose);
    std::cout << pose << std::endl;

    _simulation._model._joints = q;
    _model.update(_simulation._model, _simulation._model._joints);

    /*
    // Get current positon
    // IK of target position
    // Interpolate
    _simulation.model.getJoints(qi);
    _simulation._model.cartToJnts(pose, qf);
    list = _simulation._movement.intp(qi, qf);

    // DK of all position
    foreach( list )
    {
      qk <-- list
      _simulation._model.jntsToCart(qk, pose);
      _model.update(pose);
    }
    */

    update();
}

// -----------------------------------------------------------------------------
void
GLWidget::moveByPose ()
{
    std::cout << "[GLWidget::moveBypose]" << std::endl;
    update();
}

// -----------------------------------------------------------------------------
void
GLWidget::updateInputPose (const QString & Input)
{
    _inputPose = Input;
}

// =============================================================================
