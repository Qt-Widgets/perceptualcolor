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

#ifndef QLCHCHROMALIGHTNESSDIAGRAMM_H
#define QLCHCHROMALIGHTNESSDIAGRAMM_H

#include <QWidget>
#include <QImage>
#include <QPixmap>

/**
 * @todo write docs TODO
 */
class QLchChromaLightnessDiagramm : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double hue READ hue WRITE setHue RESET resetHue)
    Q_PROPERTY(double chroma READ chroma WRITE setChroma RESET resetChroma)
    Q_PROPERTY(double lightness READ lightness WRITE setLightness RESET resetLightness)
    Q_PROPERTY(double maximumChroma READ maximumChroma WRITE setMaximumChroma RESET resetMaximumChroma)
    Q_PROPERTY(quint8 markerRadius READ markerRadius WRITE setMarkerRadius RESET resetMarkerRadius)
    Q_PROPERTY(quint8 markerThickness READ markerThickness WRITE setMarkerThickness RESET resetMarkerThickness)
    Q_PROPERTY(quint8 border READ border) // deduced from markerRadius and markerThickness

public:
    /**
     * Default constructor
     */
    explicit QLchChromaLightnessDiagramm(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    /**
     * Destructor
     */
    virtual ~QLchChromaLightnessDiagramm();
    
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
    
    quint8 markerRadius() const;
    
    void setMarkerRadius(const quint8 newMarkerRadius);
    
    void resetMarkerRadius();

    quint8 markerThickness() const;
    
    void setMarkerThickness(const quint8 newMarkerThickness);
    
    void resetMarkerThickness();
    
    quint8 border() const;
    
    static quint8 calculateBorder(const quint8 c_markerRadius, const quint8 c_markerThickness);

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
    Q_DISABLE_COPY(QLchChromaLightnessDiagramm)
    QPixmap diagrammPixmap;
    QImage diagrammImage;
    double m_hue;
    double m_chroma;
    double m_lightness;
    double m_maximumChroma;
    quint8 m_markerRadius;
    quint8 m_markerThickness;
    quint8 m_border;
    void refreshDiagramPixmap();
    /**
     * @return A chroma-lightness diagramm for the given hue. For the y axis, its heigth covers
     * the lightness range 0..100. [Pixel (0) corresponds to value 100. Pixel (height-1) corresponds
     * to value 0.] Its x axis uses always the same scale as the y axis.
     */
    static QImage diagramm(const double c_hue, const QSize c_size);
    void updateValues(const QPoint pos);
    bool inRange(const int low, const int x, const int high);
    const double default_hue = 34;
    const double default_chroma = 29;
    const double default_lightness = 50;
    const double default_maximumChroma = 140;
    const quint8 default_markerRadius = 4;
    const quint8 default_markerThickness = 2;
};

#endif // QLCHCHROMALIGHTNESSDIAGRAMM_H
