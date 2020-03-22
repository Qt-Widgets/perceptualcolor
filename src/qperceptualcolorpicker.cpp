// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2020 Lukas Sommer somerluk@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "qperceptualcolorpicker.h"
#include "ui_qperceptualcolorpicker.h"
#include "qlchchromalightnessdiagramm.h"
#include "qlchhuewheel.h"
#include "qlchcolorwheel.h"
#include <QVBoxLayout>
#include <QSlider>
#include <QDialog>
#include <QSpinBox>

qperceptualcolorpicker::qperceptualcolorpicker(QWidget *parent) :
    QDialog(parent)
{
    QLchChromaLightnessDiagramm *myWidget = new QLchChromaLightnessDiagramm();
    QSlider *mySlider = new QSlider(Qt::Horizontal);
    QLchColorWheel *myWheel = new QLchColorWheel(this);
    mySlider->setMinimum(0);
    mySlider->setMaximum(360);
    mySlider->setValue(myWidget->hue());
    QSpinBox *myWheelValue = new QSpinBox();
    myWheelValue->setMaximum(360);
    QSpinBox *mySliderValue = new QSpinBox();
    mySliderValue->setMaximum(360);
    connect(mySlider, &QSlider::valueChanged, myWidget, &QLchChromaLightnessDiagramm::setHue);
    connect(myWheel, &QLchColorWheel::hueChanged, myWidget, &QLchChromaLightnessDiagramm::setHue);
    connect(myWheel, &QLchColorWheel::hueChanged, myWheelValue, &QSpinBox::setValue);
    connect(mySlider, &QSlider::valueChanged, mySliderValue, &QSpinBox::setValue);
//     connect(mySlider, SIGNAL(destroyed(QObject*)), this, SLOT(objectDestroyed(Qbject*))); TODO
    QSlider *mySlider2 = new QSlider(Qt::Horizontal);
    QVBoxLayout *mainLayout = new QVBoxLayout;
//     mainLayout->addWidget(myWidget);
    mainLayout->addWidget(myWheel);
    mainLayout->addWidget(myWheelValue);
    mainLayout->addWidget(mySlider);
    mainLayout->addWidget(mySliderValue);
    mainLayout->addWidget(mySlider2);
    setLayout(mainLayout);
    show();
}

qperceptualcolorpicker::~qperceptualcolorpicker() = default;
