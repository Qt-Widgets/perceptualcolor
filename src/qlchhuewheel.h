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

#ifndef QLCHHUEWHEEL_H
#define QLCHHUEWHEEL_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include "qpolardegreepointf.h"
#include "ColorSpace.h"

/**
 * @todo write docs TODO
 */
class QLchHueWheel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double hue READ hue WRITE setHue RESET resetHue NOTIFY hueChanged)
    Q_PROPERTY(double chroma READ chroma WRITE setChroma RESET resetChroma)
    Q_PROPERTY(double lightness READ lightness WRITE setLightness RESET resetLightness)
    Q_PROPERTY(double maximumChroma READ maximumChroma WRITE setMaximumChroma RESET resetMaximumChroma)
    Q_PROPERTY(int markerThickness READ markerThickness WRITE setMarkerThickness RESET resetMarkerThickness)
    Q_PROPERTY(int wheelThickness READ wheelThickness WRITE setWheelThickness RESET resetWheelThickness)
    Q_PROPERTY(int border READ border) // deduced from markerThickness and wheelThickness
    Q_PROPERTY(int wheelDiameter READ wheelDiameter);
public:
    /**
     * Default constructor
     */
    explicit QLchHueWheel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    /**
     * Destructor
     */
    virtual ~QLchHueWheel();
    
    /**
     * @return Get the hue property. Guarantied: 0 <= hue < 360.
     */
    double hue() const;

    /**
     * @return Get the chroma property. Guarantied to be >= 0.
     */
    double chroma() const;

    /**
     * @return Get the lightness property. Guarantied to be 0 <= lightness <= 100.
     */
    double lightness() const;

    /**
     * @return Get the maximum chroma that is diagram is calculated.
     */
    double maximumChroma() const;

signals:
    void hueChanged(const double &hue);

public slots:
    /**
     * Set the hue property. The hue property is the hue angle degree in the range from 0 to 360, where the
     * circle is again at its beginning. The value is gets normalized to this range. So
     * \li 0 gets 0
     * \li 359.9 gets 359.9
     * \li 360 gets 0
     * \li 361.2 gets 1.2
     * \li 720 gets 0
     * \li -1 gets 359
     * \li -1.3 gets 358.7
     * After changing the hue property, the widget gets updated.
     * @param newHue The new hue value to set.
     */
    void setHue(const double newHue);

    /**
     * Set the chroma property. After changing this hue property, the widget gets updated.
     * @param newChroma The new chroma value to set. Range: >= 0. Values < 0 will be substituted by 0.
     */
    void setChroma(const double newChroma);

    /**
     * Set the lightness property. After changing this hue property, the widget gets updated.
     * @param newLightness The new lightness value to set. Range: 0..100. Values out of this range will be bound to this range.
     */
    void setLightness(const double newLightness);
    /**
     * Set the chroma property.
     */
//     void setChroma(const double newChroma);

    /**
     * Set the lightness property.
     */
//     void setLightness(const double newHue);

    /**
     * Set the maximum chroma that will be displayed in the diagram. Must be 10 <= maximumChroma <= 230 (useful range).
     * @param newMaximumChroma The new maximum chroma value to set.
     */
    void setMaximumChroma(const double newMaximumChroma);
    
    void resetHue();
    
    void resetChroma();
    
    void resetLightness();
    
    void resetMaximumChroma();

    virtual QSize sizeHint() const;

    virtual QSize minimumSizeHint() const;
    
    quint8 markerThickness() const;
    
    void setMarkerThickness(const quint8 newMarkerThickness);
    
    void resetMarkerThickness();

    quint8 wheelThickness() const;
    
    void setWheelThickness(const quint8 newMarkerThickness);
    
    void resetWheelThickness();
    
    int border() const;

    int wheelDiameter() const;
    
    static quint8 calculateBorder(const quint8 c_markerThickness, const quint8 c_wheelThickness);

protected:
    /**
     * @todo write docs
     *
     * @param event TODO
     * @return TODO
     */
    virtual void paintEvent(QPaintEvent* event);

    /**
     * @todo write docs
     *
     * @param event TODO
     * @return TODO
     */
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Q_DISABLE_COPY(QLchHueWheel)
    QPixmap diagrammPixmap;
    QImage diagrammImage;
    double m_hue;
    double m_chroma;
    double m_lightness;
    double m_maximumChroma;
    int m_markerThickness;
    int m_wheelThickness;
    int m_border;
    int m_wheelDiameter;
    bool m_captureMouseMove;
    void refreshDiagramPixmap();
    /**
     * @return A QImage in the format QImage::Format_ARGB32 that contains a color wheel. Its
     * size is outerDiameter * outerDiameter. You can specify an overlap > 0 if you want some more pixels be drawn at the inner and the outer border. Non-used pixels have undefined color.
     */
    static QImage colorWheel(const quint16 outerDiameter, const quint16 thickness, const quint16 overlap);
    /**
     * @return An antialised color wheel image. The background is transparent.
     */
    static QImage antialisedColorWheel(const quint16 outerDiameter, const quint16 thickness);
    QPolarDegreePointF toPolarDegreeCoordinates(const QPoint windowCoordinates) const;
    void updateWheelDiameter();
    const double default_hue = 34;
    static constexpr double default_chroma = 29;
    static constexpr double default_lightness = 50;
    const double default_maximumChroma = 140;
    const quint8 default_markerThickness = 2;
    const quint8 default_wheelThickness = 20;
};

#endif // QLCHHUEWHEEL_H
