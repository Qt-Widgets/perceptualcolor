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

#ifndef WHEELCOLORPICKER_H
#define WHEELCOLORPICKER_H

#include "PerceptualColor/chromalightnessdiagram.h"
#include "PerceptualColor/simplecolorwheel.h"

#include <QDebug>

// TODO Add whatsThis value explaining the accepted keys and mouse movements

namespace PerceptualColor {

/** @brief Complete wheel-based color picker widget
 * 
 * This is a composite widget: It inherits from the SimpleColorWheel() widget
 * and adds a ChromaLightnessDiagram() in its center.
 */
class WheelColorPicker : public SimpleColorWheel
{
    Q_OBJECT

    /** @brief Currently selected color
     * 
     * This property is provides as an RGB value.
     * 
     * @sa currentColorRgb()
     * @sa setCurrentColorRgb()
     * @sa currentColorRgbChanged()
     */
     Q_PROPERTY(PerceptualColor::FullColorDescription currentColor
        READ currentColor
        WRITE setCurrentColor
        NOTIFY currentColorChanged
        USER true
    )
     // TODO The base class has yet a property with "USER true": the hue property.
     // Now currentColorRgb is added.
     // Can more than one USER property co-exist?

public:
    explicit WheelColorPicker(RgbColorSpace *colorSpace, QWidget *parent = nullptr);
    virtual ~WheelColorPicker() = default;
    FullColorDescription currentColor();
    void setCurrentColor(const FullColorDescription &newCurrentColorRgb);

Q_SIGNALS:
    /** @brief Signal for currentColor() property */
    void currentColorChanged(const PerceptualColor::FullColorDescription &newCurrentColor);
    
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    Q_DISABLE_COPY(WheelColorPicker)
    /** @brief A pointer to the inner ChromaLightnessDiagram() widget. */
    ChromaLightnessDiagram *m_chromaLightnessDiagram;
    void resizeChildWidget();
    static QSize scaleRectangleToDiagonal(const QSize oldRectangle, const qreal newDiagonal);
    
private Q_SLOTS:
    void scheduleUpdate();
};

}

#endif // WHEELCOLORPICKER_H
