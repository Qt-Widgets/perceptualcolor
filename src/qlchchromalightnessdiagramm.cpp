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

#include "qlchchromalightnessdiagramm.h"
#include "qpolardegreepointf.h"
#include "helper.h"
#include <math.h>
#include "Conversion.h"
#include "ColorSpace.h"
#include <QPainter>
#include <QtMath>
#include <QDebug>
#include <QMouseEvent>

QLchChromaLightnessDiagramm::QLchChromaLightnessDiagramm(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    // Initial default values
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAttribute(Qt::WA_OpaquePaintEvent); // As we repaint the widget entirely anyway, we can safely set this attribute which will hopefully result in a performance gain.
    resetHue();
    resetChroma();
    resetLightness();
    resetMaximumChroma();
    resetMarkerRadius();
    resetMarkerThickness();
    refreshDiagramPixmap(); // TODO is it a good idea to do that here? Or rather not? How to initialize diagrammPixmap?
    update(); // TODO is it a good idea to do that here? Or rather not?
}

QLchChromaLightnessDiagramm::~QLchChromaLightnessDiagramm()
{
}

quint8 QLchChromaLightnessDiagramm::calculateBorder(const quint8 c_markerRadius, const quint8 c_markerThickness)
{
    return qRound(c_markerRadius + (c_markerThickness / static_cast<double>(2)));
}

void QLchChromaLightnessDiagramm::updateValues(const QPoint pos)
{
    int myX = qMin(pos.x(), width() - 1 - m_border); // treat positions that are too much at the right (outside the diagram) as they where just within the diagram. It is not necessary to do the same for the left site, as setChroma() substitutes negative values with "0" anyway.
    setChroma((static_cast<double>(myX - m_border) * 100) / (diagrammPixmap.height() - 1)); // TODO: Test if this is really within the widget displayed. Otherwiese: The marker must stand at the right of the widget!
    setLightness((static_cast<double>(pos.y() - m_border) * 100) / (diagrammPixmap.height() - 1)  * (-1) + 100); // no further check of the position is necessary, because setLightness() bounds the value between 0..100 anyway.
}

void QLchChromaLightnessDiagramm::mousePressEvent(QMouseEvent *event)
{
    updateValues(event->pos());
    setCursor(Qt::BlankCursor);
}

bool QLchChromaLightnessDiagramm::inRange(const int low, const int x, const int high)
{
    return (low <= x && x <= high);
}

void QLchChromaLightnessDiagramm::mouseMoveEvent(QMouseEvent *event)
{
    updateValues(event->pos());
    if (inRange(m_border, event->pos().x(), size().width() - m_border) && inRange(m_border, event->pos().y(), size().height() - m_border)) { // mouse within our widget
        setCursor(Qt::BlankCursor);
    } else {
        unsetCursor();
    }
}

void QLchChromaLightnessDiagramm::mouseReleaseEvent(QMouseEvent *event)
{
    updateValues(event->pos());
    unsetCursor();
}

// TODO also keyboard events, not only mouse!

void QLchChromaLightnessDiagramm::paintEvent(QPaintEvent* event)
{
    QColor currentBackgroundStyle = palette().color(QPalette::Window);

    // paint the background
    QPainter painter(this);
    painter.fillRect(
        0,
        0,
        size().width(),
        size().height(),
        currentBackgroundStyle  // We specify a plain color here because some widget styles may have a pattern as a background, but that wouldn't be good for our widget (difficult to read). So we take a pain color based on the current widget style.
    );

    // paint the diagramm from the cache
    painter.drawPixmap(m_border, m_border, diagrammPixmap);

    // paint the marker
    painter.setRenderHint(QPainter::Antialiasing); // otherwise it would be ugly
    QPen pen;
    int selectPixelX = qRound(m_chroma * (diagrammPixmap.height() - 1) / 100 + m_border);
    int selectPixelY = qRound(m_lightness * (diagrammPixmap.height() - 1) / 100 * (-1) + diagrammPixmap.height() - 1 + m_border);
    pen.setWidth(m_markerThickness);
    QColor diagrammPixelColor = diagrammImage.pixelColor(selectPixelX - m_border, selectPixelY - m_border);
    int markerBackgroundLightness; // range: 0..100
    if ((diagrammPixelColor.alpha() == 0) || (!diagrammPixelColor.isValid())) {
        markerBackgroundLightness = currentBackgroundStyle.lightness() * 100 / 255;
    } else {
        markerBackgroundLightness = m_lightness;
    }
    if (markerBackgroundLightness >= 50) {
        pen.setColor(Qt::black);
    } else {
        pen.setColor(Qt::white);
    }
    painter.setPen(pen);
    painter.drawEllipse(selectPixelX -  m_markerRadius, selectPixelY - m_markerRadius, 2* m_markerRadius + 1, 2 * m_markerRadius + 1);
}

void QLchChromaLightnessDiagramm::resizeEvent(QResizeEvent* event)
{
    refreshDiagramPixmap();
    // As by Qt documentation: The widget will be erased and receive a paint event immediately after processing the resize event. No drawing need be (or should be) done inside this handler.
}

double QLchChromaLightnessDiagramm::hue() const
{
    return m_hue;
}

double QLchChromaLightnessDiagramm::chroma() const
{
    return m_chroma;
}

void QLchChromaLightnessDiagramm::setChroma(const double newChroma)
{
    double temp = qMax(static_cast<double>(0), newChroma); // substitute values < 0 with 0
    if (m_chroma != temp) {
        m_chroma = temp;
        update(); // schedule a paintEvent()
    }
}

void QLchChromaLightnessDiagramm::setLightness(const double newLightness)
{
    double temp = qBound(static_cast<double>(0), newLightness, static_cast<double>(100)); // substitute values < 0 with 0 and values > 100 with 100
    if (m_lightness != temp) {
        m_lightness = temp;
        update(); // schedule a paintEvent()
    }
}

double QLchChromaLightnessDiagramm::lightness() const
{
    return m_lightness;
}

void QLchChromaLightnessDiagramm::setHue(const double newHue)
{
    double temp = QPolarDegreePointF::normalizedAngleDegree(newHue);
    if (m_hue != temp) {
        m_hue = temp;
        refreshDiagramPixmap();
        update();
    }
}

double QLchChromaLightnessDiagramm::maximumChroma() const
{
    return m_maximumChroma;
}

void QLchChromaLightnessDiagramm::setMaximumChroma(const double newMaximumChroma)
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

QSize QLchChromaLightnessDiagramm::sizeHint() const
{
    return QSize(300, 300);
}

QSize QLchChromaLightnessDiagramm::minimumSizeHint() const
{
    return QSize(100, 100);
}

void QLchChromaLightnessDiagramm::refreshDiagramPixmap()
{
    diagrammImage = diagramm(
        m_hue,
        QSize(size().width() - 2*m_border, size().height() - 2*m_border)
    );
    diagrammPixmap = QPixmap::fromImage(diagrammImage);
}

QImage QLchChromaLightnessDiagramm::diagramm(const double c_hue, const QSize c_size) {
    QImage temp_image = QImage(c_size, QImage::Format_ARGB32);
    temp_image.fill(Qt::transparent);
    int maxHeight = c_size.height() - 1;
    int maxWidth = c_size.width() - 1;
    if ((maxHeight <= 0) || (maxWidth <= 0)) {
        // maxHeight and maxWidth must be at least >= 1 for our algorithm. If they are 0, this would crash (division by 0).
        return temp_image;
    }
    double lightness;
    double chroma;
    ColorSpace::Rgb rgb;
    for (int y = 0; y <= maxHeight; ++y) {
        lightness = y * static_cast<double>(100) / maxHeight; // floating point division thanks to 100 which is a "double"
        for (int x = 0; x <= maxWidth; ++x) {
            chroma = x * static_cast<double>(100) / maxHeight; // Using the same scale as on the y axis. floating point division thanks to 100 which is a "double"
            ColorSpace::Lch(lightness, chroma, c_hue).To<ColorSpace::Rgb>(&rgb);
            if (!helper::inGammut(rgb)) {
                /* If color is out-of-gammut: We have chroma on the x axis and
                 * lightness on the y axis. We are drawing the pixmap line per
                 * line, so we go for given lightness from low chroma to high
                 * chroma. Because of the nature of the diagram, if once in a
                 * line we have an out-of-gammut value, all other pixels that
                 * are more at the right will be out-of-gammut also. So we
                 * optimize our code and break here.
                 */
                break;
            }
            temp_image.setPixelColor(
                x,
                maxHeight - y,
                QColor::fromRgbF(rgb.r / 255, rgb.g / 255, rgb.b / 255)
            );
        }
    }
    return temp_image;
}

void QLchChromaLightnessDiagramm::resetHue()
{
    setHue(default_hue);
}

void QLchChromaLightnessDiagramm::resetChroma()
{
    setChroma(default_chroma);
}

void QLchChromaLightnessDiagramm::resetLightness()
{
    setLightness(default_lightness);
}

void QLchChromaLightnessDiagramm::resetMaximumChroma()
{
    setMaximumChroma(default_maximumChroma);
}

quint8 QLchChromaLightnessDiagramm::markerRadius() const
{
    return m_markerRadius;
}
    
void QLchChromaLightnessDiagramm::setMarkerRadius(const quint8 newMarkerRadius)
{
    if (m_markerRadius != newMarkerRadius) {
        m_markerRadius = newMarkerRadius;
        m_border = calculateBorder(m_markerRadius, m_markerThickness);
        refreshDiagramPixmap(); // because the border has changed, so the size of the pixmap will change.
        update();
    }
}
    
void QLchChromaLightnessDiagramm::resetMarkerRadius()
{
    setMarkerRadius(default_markerRadius);
}

quint8 QLchChromaLightnessDiagramm::markerThickness() const
{
    return m_markerThickness;
}
    
void QLchChromaLightnessDiagramm::setMarkerThickness(const quint8 newMarkerThickness)
{
        if (m_markerThickness != newMarkerThickness) {
            m_markerThickness = newMarkerThickness;
            m_border = calculateBorder(m_markerRadius, m_markerThickness);
            refreshDiagramPixmap(); // because the border has changed, so the size of the pixmap will change.
            update();
        }
}

void QLchChromaLightnessDiagramm::resetMarkerThickness()
{
    setMarkerThickness(default_markerThickness);
}

quint8 QLchChromaLightnessDiagramm::border() const
{
    return m_border;
}
