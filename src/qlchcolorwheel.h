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

#ifndef QLCHCOLORWHEEL_H
#define QLCHCOLORWHEEL_H

#include "qlchhuewheel.h"
#include "qlchchromalightnessdiagramm.h"

/**
 * @todo write docs
 */
class QLchColorWheel : public QLchHueWheel
{
    Q_OBJECT

public:
    /**
     * Default constructor
     */
    explicit QLchColorWheel(QWidget *parent, Qt::WindowFlags f = Qt::WindowFlags());
    
protected:
    virtual void resizeEvent(QResizeEvent* event);

private:
    Q_DISABLE_COPY(QLchColorWheel)
    static QSize resizeToDiagonal(const QSize rectangle, const quint16 newDiagonal);
    void resizeChildWidget();
    QLchChromaLightnessDiagramm *m_diagramm;
};

#endif // QLCHCOLORWHEEL_H
