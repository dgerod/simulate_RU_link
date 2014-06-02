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
#ifndef APP_GLWIDGET_H
#define APP_GLWIDGET_H

#include <iostream>
#include <QGLWidget>
#include <QTimer>
#include <GL/glut.h>
#include "helpers.h"
#include "Graphics.h"
#include "Simulation.h"

class GLWidget : public QGLWidget
{
Q_OBJECT

public:
    GLWidget (QWidget* Parent = 0);
    ~GLWidget ();

    QSize minimumSizeHint () const;
    QSize sizeHint () const;

public slots:
    void setXRotation (int Angle);
    void setYRotation (int Angle);
    void setZRotation (int Angle);
    void setZoom (int Increment);

    void updatePosInput (const QString& Input);
    void activeMovement (bool IsActive);
    void moveByJoints ();
    void moveByPose ();

    void changeStatusByJoints ();
    void executeMoveByJoints ();
    void executeMovement ();

signals:
    void xRotationChanged (int Angle);
    void yRotationChanged (int Angle);
    void zRotationChanged (int Angle);
    void zoomChanged (int Increment);

    void writeSolution (const QString &);
    void showMessage (const QString &);

protected:
    void initializeGL ();
    void paintGL ();
    void resizeGL (int Width, int Height);

    void mousePressEvent (QMouseEvent* Event);
    void mouseMoveEvent (QMouseEvent* Event);

private:
    void normalizeAngle(int *angle);
    void displayText( const char *fmt, ... );

    // Scene management
    // -----------------------------------

    // Input information
    QString _inputPose;
    bool _isMovementActive;

    QTimer* _timer;

    // Camera
    float _camPos[3];
    int _xRot, _yRot, _zRot, _zoom;
    QPoint _lastPos;

    //
    // -----------------------------------

    Graphics _gfx;
    Simulation _sim;
};

#endif
// =============================================================================

