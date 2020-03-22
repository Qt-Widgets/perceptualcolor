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

#include "qlchhuewheel.h"
#include "qpolardegreepointf.h"
#include "qlchchromalightnessdiagramm.h"
#include "helper.h"
#include <math.h>
#include "Conversion.h"
#include "ColorSpace.h"
#include <QPainter>
#include <QtMath>
#include <QDebug>
#include <QMouseEvent>

QLchHueWheel::QLchHueWheel(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    // Initial default values
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAttribute(Qt::WA_OpaquePaintEvent); // As we repaint the widget entirely anyway, we can safely set this attribute which will hopefully result in a performance gain.
    resetHue();
    resetChroma();
    resetLightness();
    resetMaximumChroma();
    resetMarkerThickness();
    resetWheelThickness();
    updateWheelDiameter();
    m_captureMouseMove = false;
    refreshDiagramPixmap(); // TODO is it a good idea to do that here? Or rather not? How to initialize diagrammPixmap?
    update(); // TODO is it a good idea to do that here? Or rather not?
}

QLchHueWheel::~QLchHueWheel()
{
}

quint8 QLchHueWheel::calculateBorder(const quint8 c_markerThickness, const quint8 c_wheelThickness)
{
    return qRound(c_markerThickness + (c_wheelThickness / static_cast<double>(2)));
}

QPolarDegreePointF QLchHueWheel::toPolarDegreeCoordinates(const QPoint windowCoordinates) const
{
    double radius = m_wheelDiameter / static_cast<double>(2);
    double myX = windowCoordinates.x() - radius;
    double myY = radius - windowCoordinates.y();
    return QPolarDegreePointF(QPointF(myX, myY));
}

void QLchHueWheel::updateWheelDiameter()
{
    m_wheelDiameter = qMin(size().width(), size().height());
}

void QLchHueWheel::mousePressEvent(QMouseEvent *event)
{
    double radius = m_wheelDiameter / static_cast<double>(2);
    QPolarDegreePointF myPolarPoint = toPolarDegreeCoordinates(event->pos());
    if (helper::inDoubleRange(radius-m_wheelThickness, myPolarPoint.radial(), radius)) {
        m_captureMouseMove = true;
        setHue(myPolarPoint.angleDegree());
    }
}

void QLchHueWheel::mouseMoveEvent(QMouseEvent *event)
{
    if (m_captureMouseMove) {
        setHue(toPolarDegreeCoordinates(event->pos()).angleDegree());
    }
}

void QLchHueWheel::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_captureMouseMove) {
        setHue(toPolarDegreeCoordinates(event->pos()).angleDegree());
        m_captureMouseMove = false;
    }
}

// TODO also keyboard events, not only mouse!

void QLchHueWheel::paintEvent(QPaintEvent* event)
{
    QColor currentBackgroundStyle = palette().color(QPalette::Window);

    // paint the background
    QPainter painter(this);
    painter.fillRect(
        0,
        0,
        size().width(),
        size().height(),
        currentBackgroundStyle  // We specify a plain color here because some widget styles may have a pattern as a background, but that wouldn't be good for our widget (difficult to read). So we take a plain color based on the current widget style.
    );

    // paint the wheel from the cache
    painter.drawPixmap(0, 0, diagrammPixmap);
    
    // paint the marker
    double radius = m_wheelDiameter / static_cast<double>(2);
    // get cartesian coordinates for our marker (asuming the pole of the coordinate system being in the center of the circle)
    QPointF myMarkerOuter = QPolarDegreePointF(
        radius,
        m_hue
    ).toQPointF();
    QPointF myMarkerInner = QPolarDegreePointF(
        radius - m_wheelThickness,
        m_hue
    ).toQPointF();
    // transform the coordinate system to Qt's default system for the paint engine
    myMarkerOuter.setX(myMarkerOuter.x() + radius);
    myMarkerOuter.setY(radius - myMarkerOuter.y());
    myMarkerInner.setX(myMarkerInner.x() + radius);
    myMarkerInner.setY(radius - myMarkerInner.y());
    // draw the line
    QPen pen;
    pen.setWidth(m_markerThickness);
    pen.setCapStyle(Qt::FlatCap);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawLine(myMarkerInner, myMarkerOuter);
}

void QLchHueWheel::resizeEvent(QResizeEvent* event)
{
    updateWheelDiameter();
    refreshDiagramPixmap();
    // As by Qt documentation: The widget will be erased and receive a paint event immediately after processing the resize event. No drawing need be (or should be) done inside this handler.
}

double QLchHueWheel::hue() const
{
    return m_hue;
}

double QLchHueWheel::chroma() const
{
    return m_chroma;
}

void QLchHueWheel::setChroma(const double newChroma)
{
    double temp = qMax(static_cast<double>(0), newChroma); // substitute values < 0 with 0
    if (m_chroma != temp) {
        m_chroma = temp;
        update(); // schedule a paintEvent()
    }
}

void QLchHueWheel::setLightness(const double newLightness)
{
    double temp = qBound(static_cast<double>(0), newLightness, static_cast<double>(100)); // substitute values < 0 with 0 and values > 100 with 100
    if (m_lightness != temp) {
        m_lightness = temp;
        update(); // schedule a paintEvent()
    }
}

double QLchHueWheel::lightness() const
{
    return m_lightness;
}

void QLchHueWheel::setHue(const double newHue)
{
    double temp = QPolarDegreePointF::normalizedAngleDegree(newHue);
    if (m_hue != temp) {
        m_hue = temp;
        refreshDiagramPixmap();
        emit hueChanged(m_hue);
        update();
    }
}

double QLchHueWheel::maximumChroma() const
{
    return m_maximumChroma;
}

void QLchHueWheel::setMaximumChroma(const double newMaximumChroma)
{
    double temp = newMaximumChroma;
    if (temp < 10) {
        temp = 10;
    }
    if (temp > 230) {
        temp = 230; // Is 230 the real maximul chroma of the Lch gammut itself?
    }
    if (m_maximumChroma != temp) {
        m_maximumChroma = temp;
        refreshDiagramPixmap();
        update();
    }
}

QSize QLchHueWheel::sizeHint() const
{
    return QSize(300, 300);
}

int QLchHueWheel::wheelDiameter() const
{
    return m_wheelDiameter;
}

QSize QLchHueWheel::minimumSizeHint() const
{
    return QSize(100, 100);
}

void QLchHueWheel::refreshDiagramPixmap()
{
    diagrammImage = antialisedColorWheel(
        qMin(size().width(), size().height()),
        m_wheelThickness
    );
    diagrammPixmap = QPixmap::fromImage(diagrammImage);
}

QImage QLchHueWheel::antialisedColorWheel(const quint16 outerDiameter, const quint16 thickness)
{
    // get a non-antialised color wheel, but with 2 pixels extra at the inner and outer side
    QImage rawWheel = colorWheel(
        outerDiameter,
        thickness,
        2
    );
    
    // construct our final QImage with transparent background
    QImage finalWheel = QImage(QSize(outerDiameter, outerDiameter), QImage::Format_ARGB32);
    finalWheel.fill(Qt::transparent);
    
    // paint an anti-alised circle with the color wheel as brush
    QPainter myPainter(&finalWheel);
    myPainter.setRenderHint(QPainter::Antialiasing, true);
    myPainter.setPen(QPen(Qt::NoPen));
    myPainter.setBrush(QBrush(rawWheel));
    myPainter.drawEllipse(0, 0, outerDiameter, outerDiameter);
    
    // set the inner circle of the wheel to antialised transparency
    myPainter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    myPainter.setRenderHint(QPainter::Antialiasing, true);
    myPainter.setPen(QPen(Qt::NoPen));
    myPainter.setBrush(QBrush(Qt::SolidPattern));
    myPainter.drawEllipse(thickness, thickness, outerDiameter - 2 * thickness, outerDiameter - 2 * thickness);
    
    // return
    return finalWheel;
}

QImage QLchHueWheel::colorWheel(const quint16 outerDiameter, const quint16 thickness, const quint16 overlap)
{
    // The overlap defines an overlap for the wheel, so there are some more pixels that are drawn at the outer and at the inner border of the wheel, to allow later clipping with antialising
    QPolarDegreePointF polarCoordinates;
    ColorSpace::Rgb rgb;
    QColor color;
    int x;
    int y;
    quint16 maxExtension = outerDiameter - 1;
    double center = maxExtension / static_cast<double>(2);
    QImage image = QImage(QSize(outerDiameter, outerDiameter), QImage::Format_ARGB32);
    for (x = 0; x <= maxExtension; ++x) {
        for (y = 0; y <= maxExtension; ++y) {
            polarCoordinates = QPolarDegreePointF(QPoint(x - center, center - y));
            if (helper::inDoubleRange(center - thickness - overlap, polarCoordinates.radial(), center + overlap)) {
                // We are within the wheel
                ColorSpace::Lch(
                    default_lightness,
                    default_chroma,
                    polarCoordinates.angleDegree()
                ).To<ColorSpace::Rgb>(&rgb);
                if (helper::inGammut(rgb)) {
                    image.setPixelColor(
                        x,
                        y,
                        QColor::fromRgbF(rgb.r / 255, rgb.g / 255, rgb.b / 255)
                    );
                }
            }
        }
    }
    return image;
}

void QLchHueWheel::resetHue()
{
    setHue(default_hue);
}

void QLchHueWheel::resetChroma()
{
    setChroma(default_chroma);
}

void QLchHueWheel::resetLightness()
{
    setLightness(default_lightness);
}

void QLchHueWheel::resetMaximumChroma()
{
    setMaximumChroma(default_maximumChroma);
}

quint8 QLchHueWheel::markerThickness() const
{
    return m_markerThickness;
}
    
void QLchHueWheel::setMarkerThickness(const quint8 newMarkerThickness)
{
    if (m_markerThickness != newMarkerThickness) {
        m_markerThickness = newMarkerThickness;
        m_border = calculateBorder(m_markerThickness, m_wheelThickness);
        refreshDiagramPixmap(); // because the border has changed, so the size of the pixmap will change.
        update();
    }
}
    
void QLchHueWheel::resetMarkerThickness()
{
    setMarkerThickness(default_markerThickness);
}

quint8 QLchHueWheel::wheelThickness() const
{
    return m_wheelThickness;
}
    
void QLchHueWheel::setWheelThickness(const quint8 newWheelThickness)
{
        if (m_wheelThickness != newWheelThickness) {
            m_wheelThickness = newWheelThickness;
            refreshDiagramPixmap(); // because the border has changed, so the size of the pixmap will change.
            update();
        }
}

void QLchHueWheel::resetWheelThickness()
{
    setWheelThickness(default_wheelThickness);
}

int QLchHueWheel::border() const
{
    return m_border;
}
