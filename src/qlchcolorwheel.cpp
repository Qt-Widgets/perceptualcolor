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

#include "qlchcolorwheel.h"
#include "qlchchromalightnessdiagramm.h"
#include <math.h>
#include <QDebug>

QLchColorWheel::QLchColorWheel(QWidget *parent, Qt::WindowFlags f) : QLchHueWheel(parent, f)
{
    m_diagramm = new QLchChromaLightnessDiagramm(this);
    m_diagramm->show();
    resizeChildWidget();
    connect(this, &QLchColorWheel::hueChanged, m_diagramm, &QLchChromaLightnessDiagramm::setHue);
}

QSize QLchColorWheel::resizeToDiagonal(const QSize rectangle, const quint16 newDiagonal)
{
    double ratioWidthPerHeight = static_cast<double>(rectangle.width()) / rectangle.height();
    int newHeight = sqrt(pow(newDiagonal, 2) / (1 + pow(ratioWidthPerHeight, 2)));
    return QSize(newHeight * ratioWidthPerHeight, newHeight);
}

void QLchColorWheel::resizeChildWidget()
{
    int diagonal = qMax(
        wheelDiameter() - (2 * wheelThickness()),
        0
    );
    QSize newChromaLightnessDiagrammSize = resizeToDiagonal(QSize(140, 100), diagonal);
    m_diagramm->resize(newChromaLightnessDiagrammSize);
    double radius = static_cast<double>(wheelDiameter()) / 2;
    m_diagramm->move(
        radius - newChromaLightnessDiagrammSize.width() / 2,
        radius - newChromaLightnessDiagrammSize.height() / 2
    );
}

void QLchColorWheel::resizeEvent(QResizeEvent* event)
{
    QLchHueWheel::resizeEvent(event);
    resizeChildWidget();
}

