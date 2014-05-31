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

#include <QDesktopWidget>
#include <QMainWindow>
#include "window.h"

#include <kdl/chain.hpp>
#include <kdl/chainfksolver.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/frames_io.hpp>
#include <stdio.h>
#include <iostream>

// -----------------------------------------------------------------------------
qtWindow::qtWindow()
{
    glWidget = new GLWidget;
    leInputInfo = new QLineEdit("0.0, 0.0, 0.0");
    btDkMove = new QPushButton("Joints");
    btIkMove = new QPushButton("Pose");
    click = new QTimer(this);

    xSlider = createSlider();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);

    QGridLayout *kk = new QGridLayout;

    kk->addWidget(leInputInfo);
    kk->addWidget(btDkMove);
    kk->addWidget(btIkMove);
    mainLayout->addLayout(kk);

    mainLayout->addWidget(xSlider);
    setLayout(mainLayout);

    //connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
    //connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));

    connect(leInputInfo, SIGNAL(textChanged(const QString &)), glWidget, SLOT(updateInputPose(const QString &)));
    connect(btIkMove, SIGNAL(clicked()), glWidget, SLOT(moveByPose()));
    connect(btDkMove, SIGNAL(clicked()), glWidget, SLOT(moveByJoints()));
    connect(click, SIGNAL(timeout()), glWidget, SLOT(updateGL()));

    //xSlider->setValue(0 * 16);

    //QDesktopWidget dw;
    //QRect screenSize = dw.availableGeometry(this);
    //setFixedSize(screenSize.width() * 0.7f, screenSize.height() * 0.7f);

    setWindowTitle(tr("Simulate RU link"));
    click->start(1000);
}

// -----------------------------------------------------------------------------
QSlider *qtWindow::createSlider ()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

// =============================================================================