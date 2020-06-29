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

#ifndef SIMPLECOLORWHEEL_H
#define SIMPLECOLORWHEEL_H

#include <QImage>
#include <QWidget>

#include "PerceptualColor/polarpointf.h"
#include "PerceptualColor/rgbcolorspace.h"

#include <lcms2.h>

namespace PerceptualColor {

/** @brief Widget to choose the hue in a wheel.
 * 
 * This widget allows to select the hue (as defined in the LCh color space) in the form of a wheel.
 */
class SimpleColorWheel : public QWidget
{
    Q_OBJECT

    /** // deduced from markerThickness and wheelThickness */
    /** @brief The border between the outer border of the wheel ribbon and the
     * border of the widget.
     * 
     * The diagram is not painted on the whole extend of the widget. A border is
     * left to allow that the focus indicatorcan be painted completly even when
     * the widget has the focus. The border is determined automatically, its
     * value depends on markerThickness().
     * 
     * @sa border()
     */
    Q_PROPERTY(int border READ border)

    /** @brief The currently selected hue.
     * 
     * The hue property is the LCh hue angle (measured in degree) The value
     * gets normalized according to PolarPointF::normalizedAngleDegree()
     * 
     * After changing the hue property, the widget gets updated.
     * 
     * @sa hue()
     * @sa setHue()
     * @sa resetHue()
     * @sa hueChanged()
     * @sa default_hue
     */
    Q_PROPERTY(qreal hue READ hue WRITE setHue RESET resetHue NOTIFY hueChanged USER true)

    /** @brief the thickness of the marker
     * 
     * @sa markerThickness()
     * @sa setMarkerThickness()
     * @sa resetMarkerThickness()
     * @sa default_markerThickness
     */
    Q_PROPERTY(int markerThickness READ markerThickness WRITE setMarkerThickness RESET resetMarkerThickness)

    /** @brief the chroma with which the wheel ribbon is painted.
     * 
     * @sa wheelRibbonChroma()
     * @sa setWheelRibbonChroma()
     * @sa resetWheelRibbonChroma
     */
    Q_PROPERTY(qreal wheelRibbonChroma READ wheelRibbonChroma WRITE setWheelRibbonChroma RESET resetWheelRibbonChroma)

    /** @brief the thickness of the wheel ribbon
     * 
     * @sa wheelThickness()
     * @sa setWheelThickness()
     * @sa resetWheelThickness()
     * @sa default_wheelThickness
     */
    Q_PROPERTY(int wheelThickness READ wheelThickness WRITE setWheelThickness RESET resetWheelThickness)

public:
    explicit SimpleColorWheel(RgbColorSpace *colorSpace, QWidget *parent = nullptr);
    virtual ~SimpleColorWheel() override = default;
    int border() const;
    virtual bool hasHeightForWidth() const override;
    virtual int	heightForWidth(int width) const override;
    qreal hue() const;
    int markerThickness() const;
    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;
    int wheelThickness() const;
    qreal wheelRibbonChroma() const;
    static QImage generateWheelImage(
        RgbColorSpace *colorSpace,
        const int outerDiameter,
        const int border,
        const int thickness,
        const qreal lightness,
        const qreal chroma
    );

Q_SIGNALS:
    /** @brief Signal for hue() property. */
    void hueChanged(const qreal hue);

public Q_SLOTS:
    void resetHue();
    void resetMarkerThickness();
    void resetWheelRibbonChroma();
    void resetWheelThickness();
    void setHue(const qreal newHue);
    void setMarkerThickness(const int newMarkerThickness);
    void setWheelRibbonChroma(const qreal newChroma);
    void setWheelThickness(const int newMarkerThickness);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    int contentDiameter() const;

private:
    Q_DISABLE_COPY(SimpleColorWheel)

    /** @brief If a mouse event is active
     * 
     * Holds if currently a mouse event is active or not.
     * @sa mousePressEvent()
     * @sa mouseMoveEvent()
     * @sa mouseReleaseEvent()
     */ 
    bool m_mouseEventActive;
    /** @brief A cache for the wheel picture as QImage. Might be outdated!
     *  @sa updateWheelImage() 
     *  @sa m_wheelImageReady() */
    QImage m_wheelImage;
    /** Holds wether or not m_wheelImage is up-to-date.
     *  @sa refreshWheelImage()
     *  @sa updateWheelImage */
    bool m_wheelImageReady = false;
    /** @brief Internal storage of the hue() property */
    qreal m_hue;
    /** @brief Internal storage of the markerThickness() property */
    int m_markerThickness;
    /** @brief Pointer to RgbColorSpace() object */
    RgbColorSpace *m_rgbColorSpace;
    /** @brief Internal storage of the wheelRibbonChroma() property */
    qreal m_wheelRibbonChroma;
    /** @brief Internal storage of the wheelThickness() property */
    int m_wheelThickness;

    /** @brief Default value for markerThickness() property. */
    static constexpr int default_markerThickness = 2;
    /** @brief Default value for wheelThickness() property. */
    static constexpr int default_wheelThickness = 20;

    QPointF fromWheelCoordinatesToWidgetCoordinates(const PolarPointF wheelCoordinates) const;
    PolarPointF fromWidgetCoordinatesToWheelCoordinates(const QPoint widgetCoordinates) const;
    void updateWheelImage();

};

}

#endif // SIMPLECOLORWHEEL_H
