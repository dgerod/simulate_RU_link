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

    txInputInfo = new QLineEdit("0.0, 0.0, 0.0");
    btDkMove = new QPushButton("Joints");
    btIkMove = new QPushButton("Pose");
    rbDoMovement = new QRadioButton("Execute movement");
    txSolutionInfo = new QLineEdit("");
    txSolutionInfo->setReadOnly(true);
    txSolutionInfo->setStyleSheet( QString("background-color: beige") );
    txInfoMsg = new QLineEdit("OK");
    txInfoMsg->setReadOnly(true);
    txInfoMsg->setStyleSheet( QString("color: blue; background: beige") );

    connect(txInputInfo, SIGNAL(textChanged(const QString &)), glWidget, SLOT(updatePosInput(const QString &)));
    connect(btIkMove, SIGNAL(clicked()), glWidget, SLOT(moveByPose()));
    connect(btDkMove, SIGNAL(clicked()), glWidget, SLOT(moveByJoints()));
    connect(rbDoMovement, SIGNAL(toggled(bool)), glWidget, SLOT(activeMovement(bool)) );
    connect(glWidget, SIGNAL(writeSolution(const QString &)), txSolutionInfo, SLOT(setText(const QString &)));
    connect(glWidget, SIGNAL(showMessage(const QString &)), txInfoMsg, SLOT(setText(const QString &)));

    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();

    xSlider->setValue(15 * 16);
    ySlider->setValue(345 * 16);
    zSlider->setValue(0 * 16);

    connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
    connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
    connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
    connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
    connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));
    connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));
   
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* layout2 = new QHBoxLayout;
    QGridLayout* layout3 = new QGridLayout;

    mainLayout->addLayout(layout2);
    mainLayout->addWidget(txInfoMsg);

    layout2->addWidget(glWidget);
    layout2->addWidget(xSlider);
    layout2->addWidget(ySlider);
    layout2->addWidget(zSlider);
    layout2->addLayout(layout3);

    layout3->addWidget(txInputInfo);
    layout3->addWidget(btDkMove);
    layout3->addWidget(btIkMove);
    layout3->addWidget(rbDoMovement);
    layout3->addWidget(txSolutionInfo);

    setLayout(mainLayout);

    setWindowTitle(tr("Simulate RU link"));
    //click->start(1000);
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

// -----------------------------------------------------------------------------
void qtWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    	{ close(); }
    else
        { QWidget::keyPressEvent(e); }
}

// =============================================================================
