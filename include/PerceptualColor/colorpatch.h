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

#ifndef COLORPATCH_H
#define COLORPATCH_H

#include <QFrame>

namespace PerceptualColor {

/** @brief A color display widget.
 * 
 * This widget simply displays a color.
 */
class ColorPatch : public QFrame
{
    Q_OBJECT

    /** @brief The color that is displayed
     * 
     * Default value is an invalid color.
     * 
     * If the color is invalid, nothing is displayed.
     */
    Q_PROPERTY(QColor color READ color WRITE setColor RESET resetColor NOTIFY colorChanged)

public:
    explicit ColorPatch(QWidget *parent = nullptr);
    virtual ~ColorPatch() override;
    QColor color() const;
    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;
    
public Q_SLOTS:
    void resetColor();
    void setColor(const QColor &newColor);

Q_SIGNALS:
    /** @brief Signal for color() property. */
    void colorChanged(const QColor &newColor);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:

    Q_DISABLE_COPY(ColorPatch)

    QBrush m_brush;
    QColor m_color;
};

}

#endif // COLORPATCH_H
