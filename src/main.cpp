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

#include "PerceptualColor/colordialog.h"
#include "PerceptualColor/polarpointf.h"
#include "PerceptualColor/rgbcolorspace.h"
#include "PerceptualColor/fullcolordescription.h"

#include <QApplication>
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QMetaMethod>
#include <QMetaObject>
#include <QSlider>

// TODO code analysis clazy cppcheck krazy

// TODO Anti-aliasing the gamut diagrams? Wouldn't this be bad for performance?

// Test if the compiler treats the source code actually as UTF8.
// A test string is converted to UTF8 code units (u8"") and each
// code unit is checked to be correct.
static_assert(
    (static_cast<quint8>(*((u8"🖌")+0)) == 0xF0) &&
        (static_cast<quint8>(*((u8"🖌")+1)) == 0x9F) &&
        (static_cast<quint8>(*((u8"🖌")+2)) == 0x96) &&
        (static_cast<quint8>(*((u8"🖌")+3)) == 0x8C) &&
        (static_cast<quint8>(*((u8"🖌")+4)) == 0x00),
    "This source code has to be read-in as UTF8 by the compiler."
);

/* TODO Make sure to always use u8"x" as string literals because L"x" and "x" have compiler-dependent encoding
 * Alternative would be to make sure the compiler actually uses yet UTF8 also for normal "x" values.
 * But that is somethat this is not required by the standard, so it might be wired to require it here.
 * Anyway, this would be a possibility to test it:
static_assert(
    (static_cast<quint8>(*((u8"🖌")+0)) == 0xF0) &&
        (static_cast<quint8>(*((u8"🖌")+1)) == 0x9F) &&
        (static_cast<quint8>(*((u8"🖌")+2)) == 0x96) &&
        (static_cast<quint8>(*((u8"🖌")+3)) == 0x8C) &&
        (static_cast<quint8>(*((u8"🖌")+4)) == 0x00),
    "Compiler must treat "string" litterals as UTF8. Example: gcc -fexec-charset=UTF-8"
);
 */

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QObject::tr(u8"Perceptual color picker"));
//     app.setLayoutDirection(Qt::RightToLeft);
    PerceptualColor::RgbColorSpace myspace;

//     QColorDialog x(QColor::fromCmyk(45, 33, 34, 48));
//     x.setOption(QColorDialog::ShowAlphaChannel);
//     x.show();
    PerceptualColor::ColorDialog w;//(QColor(107, 181, 99, 50));
    w.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
    w.show();
/*
    app.connect(
        &x,
        &QColorDialog::currentColorChanged,
        &w,
        &PerceptualColor::ColorDialog::setCurrentColor
    );
*/
    return app.exec();
}
