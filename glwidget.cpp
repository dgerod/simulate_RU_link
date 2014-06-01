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
#include <QtGlobal>
#include <QtGui>
#include <QtDebug>
#include <QTextStream>
#include <GL/glut.h>

#include "helpers.h"
#include "glwidget.h"

// -----------------------------------------------------------------------------
GLWidget::GLWidget (QWidget* Parent)
	: QGLWidget(Parent)
{
    // Camera
    _camPos[0] = 5; _camPos[1] = 5; _camPos[2] = 1;
    _xRot = 0; _yRot = 0; _zoom = 1;
    // Execute movement
    _isMovementActive = false;

    _timer = new QTimer(this);
    _timer->setInterval(10);
    connect(_timer, SIGNAL(timeout()), this, SLOT(executeMovement()));

    // Load kinematics model and prepare graphics
    _sim._model.initialize(1, 2, 1);
    _sim._move.initialize(3);

    // Frame in the originexecuteMovement
    _gfx._origin._length = 10.0;
    _gfx._origin.setPosition(0, 0, 0);
    // Represent model
    _gfx._model.initialize( _sim._model );
    //_sim._model.initialize(10, 20, 5);
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
    glClearColor(0.1, 0.1, 0.1, 0.0);	// Sets background color to blue
    glClearDepth(1.0);			// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); 	        // Enables hidden-surface removal

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
    glViewport((Width - side) / 2, (Height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, Width/Height, 1, 100);
      
    glMatrixMode(GL_MODELVIEW);	
	
    /*
    gluPerspective(90.0, Width/Height, 1, 100);
    glViewport(0, 0, Width, Height);
    */
}

// -----------------------------------------------------------------------------
void
GLWidget::paintGL ()
{
    // Prepare the scene
    // ----------------------------------------------

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set view
    // ----------------------------------------------

    gluLookAt(_camPos[0],_camPos[1],_camPos[2],0,0,0,0,0,1);

    // Rotate when user changes rotate_x and rotate_y
    glRotated(_xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(_yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(_zRot / 16.0, 0.0, 0.0, 1.0);
    // Zoom in and out according to mouse wheel movement
    glScalef(_zoom, _zoom, 1.0f);

    // Draw scene
    // ----------------------------------------------

    // Update model
    _gfx._model.update(_sim._model, _sim._model._joints);

     // And draw all
    _gfx._floor.drawIt();
    //_gfx._origin.drawIt();
    _gfx._model.drawIt();

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
    _inputPose = Input;
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
        emit changeStatusByJoints();
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
   std::cout << "[GLWidget::executeMoveByJoints] begin" << std::endl;

   QString msg;
   int numJoints;
   double inputs[3];

   numJoints = _sim._model._chain.getNrOfJoints();

   // Validate input and prepare joints
   // ----------------------------------------------------

   if( proc_position_input(_inputPose, numJoints, inputs) == false)
   {
       emit showMessage("ERROR - Incorrect input for DK");
       return;
   }

   KDL::JntArray qf(numJoints);
   KDL::Frame frame;

   for(unsigned int jdx=0; jdx<numJoints; jdx++)
     {  qf(jdx) = inputs[jdx]; }

   printf("qf(0) = %f, qf(1) = %f, qf(2) = %f\n", qf(0), qf(1), qf(2));

   // Prepare trajectory
   double Tt = 10;
   _sim._move.start(_sim._model._joints, qf, 10);
   // Activate time
   _timer->start();

   //update();

   std::cout << "[GLWidget::executeMoveByJoints] end" << std::endl;
}

// -----------------------------------------------------------------------------
void
GLWidget::changeStatusByJoints ()
{
    std::cout << "[GLWidget::changeStatusByJoints] begin" << std::endl;

    QString msg;
    int nj = _sim._model._chain.getNrOfJoints();
    KDL::JntArray q(nj);
    KDL::Frame frame;

    // Validate input
    // ----------------------------------------------------

    QRegExp rx("[, ]");
    QStringList list = _inputPose.split(rx, QString::SkipEmptyParts);

    if( list.length() != nj )
    {
        std::cout << "ERROR: Incorrect input = " << _inputPose.toStdString() << std::endl;
        emit showMessage("ERROR - Incorrect input for DK");
        return;
    }

    for(unsigned int idx=0; idx<list.length(); idx++)
    {
        bool validate;
        q(idx) = list[idx].toDouble(&validate);

        if( validate == false )
        {
            emit showMessage("ERROR - Incorrect input for DK");
            return;
        }
    }

    printf("q(0) = %f, q(1) = %f, q(2) = %f\n", q(0), q(1), q(2));

    //
    // ----------------------------------------------------

    if( _sim._model.jntsToCart(q, frame) == true)
    {
        _sim._model._joints = q;
        _sim._model._tcs0 = frame;

        //_gfx._model.update(_sim._model, _sim._model._joints);

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

    std::cout << "[GLWidget::changeStatusByJoints] end" << std::endl;
}

// -----------------------------------------------------------------------------
void
GLWidget::moveByPose ()
{
    QString msg;
    KDL::Frame frame;
    int nj = _sim._model._chain.getNrOfJoints();
    KDL::JntArray q(nj);

    // Validate input
    // ----------------------------------------------------

    double pose[6];
    QRegExp rx("[, ]");
    QStringList list = _inputPose.split(rx, QString::SkipEmptyParts);

    if( list.length() != 6 )
    {
        std::cout << "ERROR: Incorrect input = " << _inputPose.toStdString() << std::endl;
        emit showMessage("ERROR - Incorrect input for IK.");
        return;
    }

    for(unsigned int idx=0; idx<list.length(); idx++)
    {
        bool validate;
        pose[idx] = list[idx].toDouble(&validate);

        if( validate == false )
        {
            emit showMessage("ERROR - Incorrect input for IK.");
            return;
        }
    }

    printf("p(0) = %f, p(1) = %f, p(2) = %f, p(3) = %f, p(4) = %f, p(5) = %f\n",
        pose[0], pose[1], pose[2], pose[3], pose[4], pose[5]);

    frame.p = KDL::Vector(pose[0],pose[1],pose[2]);
    frame.M = KDL::Rotation::EulerZYX (pose[5], pose[4], pose[3]);

    // ----------------------------------------------------

    if( _sim._model.cartTojnts(frame, q) == true)
    {
        _sim._model._tcs0 = frame;
        _sim._model._joints = q;

        //_gfx._model.update(_sim._model, _sim._model._joints);

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
  std::cout << "[GLWidget::executeMovement] begin" << std::endl;

  KDL::JntArray q(_sim._model._joints);
  KDL::Frame frame;

  _sim._move.nextPosition(_sim._move._Tk + 1.0, q);

  if( _sim._model.jntsToCart(q, frame) == true)
  {
      _sim._model._joints = q;
      _sim._model._tcs0 = frame;

      printf("q(0)=%f,q(1)=%f,q(2)=%f\n", q(0), q(1), q(2));
      std::cout << frame << std::endl;

      // Check if trajectory is finished
      if( _sim._move._Tk >= _sim._move._Tt)
      {
          _timer->stop();
      }

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

  updateGL();

  std::cout << "[GLWidget::executeMovement] end" << std::endl;
}

// =============================================================================
