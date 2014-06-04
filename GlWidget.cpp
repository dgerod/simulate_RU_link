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
//
// =============================================================================

#include <math.h>
#include <QtGlobal>
#include <QtGui>
#include <QtDebug>
#include <QTextStream>
#include <GL/glut.h>
#include "helpers.h"
#include "GlWidget.h"

// -----------------------------------------------------------------------------
GLWidget::GLWidget (QWidget* Parent)
	: QGLWidget(Parent)
{
    // Camera
    _camPos[0] = 40; _camPos[1] = 40; _camPos[2] = 10;
    _xRot = 0; _yRot = 0; _zRot = 0; _zoom = 1;

    // Execute movement based on joints
    _isMovementActive = false;
    _timer = new QTimer(this);
    _timer->setInterval(500);
    QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(executeMovement()));

    // Load kinematics model and prepare graphics
    double position[3] = {10.0, 0.0, 2.0};
    _sim._model.initialize(position, 20, 5);
    _sim._move.initialize( _sim._model );

    // Frame in the originexecuteMovement
    _gfx._origin._length = 50.0;
    _gfx._origin.setPosition(0, 0, 0);
    // Represent model
    _gfx._model.initialize( _sim._model );
}

// -----------------------------------------------------------------------------
GLWidget::~GLWidget ()
{
    _timer->stop();
    delete _timer;

    makeCurrent();

    _gfx._model.finalize();
    _sim._move.finalize();
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
 GLWidget::setZRotation (int Angle)
 {
     normalizeAngle(&Angle);
     if (Angle != _zRot) 
     {
         _zRot = Angle;
         emit zRotationChanged(Angle);
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
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    //glShadeModel(GL_FLAT);
    //glCullFace(GL_BACK);  //to display back face. We can put also GL_FRONT for front faces
    //glEnable(GL_CULL_FACE);	//enables displaying of both faces of polygons. (enables previous instruction)

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluPerspective(90.0, 1, 1, 100);
}

// -----------------------------------------------------------------------------
void
GLWidget::resizeGL (int Width, int Height)
{
    int side = qMin(Width, Height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, Width/Height, 1, 100);
    glViewport((Width - side) / 2, (Height - side) / 2, side, side);
}

// -----------------------------------------------------------------------------
void
GLWidget::paintGL ()
{
    // Prepare the scene
    // ----------------------------------------------

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set view
    // ----------------------------------------------

    gluLookAt(_camPos[0],_camPos[1],_camPos[2],0,0,0,0,0,1);

    // Rotate in x, y, z
    glRotated(_xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(_yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(_zRot / 16.0, 0.0, 0.0, 1.0);
    // Zoom in/out
    glScalef(_zoom, _zoom, 1.0f);

    // Draw scene
    // ----------------------------------------------

    _gfx._model.update(_sim._model, _sim._model._joints);
    _gfx.draw();

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
        setYRotation(_yRot + 8 * dx);
    }
    else if (Event->buttons() & Qt::RightButton)
    {
        setXRotation(_xRot + 8 * dy);
        setZRotation(_zRot + 8 * dx);
    }

    _lastPos = Event->pos();
}

// -----------------------------------------------------------------------------
void
GLWidget::updatePosInput (const QString& Input)
{
    _inputPos = Input;
}

// -----------------------------------------------------------------------------
void
GLWidget::activeMovement (bool IsActive)
{
    _isMovementActive = IsActive;
}

// -----------------------------------------------------------------------------
void
GLWidget::moveByJoints ()
{
    if( _isMovementActive == false )
    {
        emit changeStateByJoints();
    }
    else
    {
        emit executeMoveByJoints();
    }
}

// -----------------------------------------------------------------------------
void
GLWidget::executeMoveByJoints ()
{
   qDebug("[GLWidget::executeMoveByJoints] start" );

   QString msg;
   int numJoints;
   double inputs[3];

   numJoints = _sim._model._chain.getNrOfJoints();

   // Validate input and prepare joints
   // ----------------------------------------------------

   if( proc_position_input(_inputPos, numJoints, inputs) == false)
   {
       emit showMessage("ERROR - Incorrect input");
       return;
   }

   KDL::JntArray qf(numJoints);
   KDL::Frame frame;

   for(unsigned int jdx=0; jdx<numJoints; jdx++)
     {  qf(jdx) = inputs[jdx]; }

   printf("qf(0) = %f, qf(1) = %f, qf(2) = %f\n", qf(0), qf(1), qf(2));

   // Prepare trajectory
   // ----------------------------------------------------

   double Tt = 10;
   _sim._move.start(_sim._model._joints, qf, 10);
   // Activate time
   _timer->start();

   qDebug( "[GLWidget::executeMoveByJoints] end" );
}

// -----------------------------------------------------------------------------
void
GLWidget::changeStateByJoints ()
{
    qDebug( "[GLWidget::changeStatusByJoints] start" );

    int numJoints;
    double inputs[3];

    numJoints = _sim._model._chain.getNrOfJoints();

    // Validate input and prepare joints
    // ----------------------------------------------------

    if( proc_position_input(_inputPos, numJoints, inputs) == false)
     {
         emit showMessage("ERROR - Incorrect input");
         return;
     }

     KDL::JntArray q(numJoints);
     KDL::Frame frame;

     for(unsigned int jdx=0; jdx<numJoints; jdx++)
       {  q(jdx) = inputs[jdx]; }

     printf("q(0) = %f, q(1) = %f, q(2) = %f\n", q(0), q(1), q(2));

    // Solve DK
    // ----------------------------------------------------

    if( _sim._model.jntsToCart(q, frame) == true)
    {
        QString msg;

        _sim._model._joints = q;
        _sim._model._tcs0 = frame;

        printf("q(0)=%f,q(1)=%f,q(2)=%f\n", q(0), q(1), q(2));
        std::cout << frame << std::endl;

        frame_to_QString(frame, msg);
        emit writeSolution(msg);
        emit showMessage("OK");
    }
    else
    {
        emit writeSolution("");
        emit showMessage("ERROR - No solution");
    }

    update();

    qDebug( "[GLWidget::changeStatusByJoints] end" );
}

// -----------------------------------------------------------------------------
void
GLWidget::moveByPose ()
{
    if( _isMovementActive == false )
    {
        emit changeStateByPose();
    }
    else
    {
        emit showMessage("ERROR - Not implemented");
    }
}

// -----------------------------------------------------------------------------
void
GLWidget::changeStateByPose ()
{
    int numJoints;
    double inputs[6];

    // Validate input and prepare pose
    // ----------------------------------------------------

    if( proc_position_input(_inputPos, 6, inputs) == false)
    {
        emit showMessage("ERROR - Incorrect input");
        return;
    }

    numJoints = _sim._model._chain.getNrOfJoints();

    KDL::Frame frame;
    KDL::JntArray q(numJoints);

    printf("p(0) = %f, p(1) = %f, p(2) = %f, p(3) = %f, p(4) = %f, p(5) = %f\n",
        inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

    frame.p = KDL::Vector(inputs[0],inputs[1],inputs[2]);
    frame.M = KDL::Rotation::EulerZYX (inputs[5], inputs[4], inputs[3]);

    // ----------------------------------------------------

    if( _sim._model.cartTojnts(frame, q) == true)
    {
        QString msg;

        _sim._model._tcs0 = frame;
        _sim._model._joints = q;

        std::cout << frame << std::endl;
        printf("q(0)=%f,q(1)=%f,q(2)=%f\n", q(0), q(1), q(2));

        joints_to_QString(q, msg);
        emit writeSolution(msg);
        emit showMessage("OK");
    }
    else
    {
        emit writeSolution("");
        emit showMessage("ERROR - No solution");
    }

    update();
}

// -----------------------------------------------------------------------------
void
GLWidget::executeMovement ()
{
  qDebug( "[GLWidget::executeMovement] start" );

  KDL::JntArray q(_sim._model._joints);
  KDL::Frame frame;

  if( _sim._move.nextPosition(_sim._move.Tk() + 1.0, q) == false )
  {
      _timer->stop();
  }

  if( _sim._model.jntsToCart(q, frame) == true)
  {
      _sim._model._joints = q;
      _sim._model._tcs0 = frame;

      printf("q(0)=%f,q(1)=%f,q(2)=%f\n", q(0), q(1), q(2));
      std::cout << frame << std::endl;

      QString msg;
      frame_to_QString(frame, msg);
      emit writeSolution(msg);
      emit showMessage("OK");
  }
  else
  {
      _timer->stop();
      emit showMessage("ERROR - Movement aborted.");
  }

  update();

  qDebug( "[GLWidget::executeMovement] end" );
}

// =============================================================================
