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

#include <QGLWidget>
#include <iostream>
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header

#include "graphics.h"
#include "simulation.h"

class GLWidget : public QGLWidget
{
Q_OBJECT

public:
    GLWidget (QWidget* Parent = 0);
    ~GLWidget ();

    QSize minimumSizeHint () const;
    QSize sizeHint () const;

public
    slots:
    void setXRotation (int Angle);
    void setYRotation (int Angle);
    void setZoom (int Increment);

    void updateInputPose (const QString &);
    void moveByJoints ();
    void moveByPose ();

    signals:
    void xRotationChanged (int Angle);
    void yRotationChanged (int Angle);
    void zoomChanged (int Increment);

protected:
    void initializeGL ();
    void paintGL ();
    void resizeGL (int Width, int Height);

    void keyPressEvent (QKeyEvent* Event);
    void mousePressEvent (QMouseEvent* Event);
    void mouseMoveEvent (QMouseEvent* Event);
    void wheelEvent (QWheelEvent* Event);

private:

    void normalizeAngle(int *angle);
    void displayText( const char *fmt, ... );

    QString _inputPose;

    // Graphics layer
    // -----------------------------------

    GLuint _base;

    // Camera
    float _camPos[3];
    int _xRot, _yRot, _zoom;
    QPoint _lastPos;

    // Scene
    gfx::Floor _floor;
    gfx::RefFrame _origin;
    gfx::Model _model;

    // Simulation engine
    // -----------------------------------

    Simulation _simulation;
};

#endif
// =============================================================================

