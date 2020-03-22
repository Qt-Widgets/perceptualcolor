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

#include "qlchchromalightnessdiagrammworkerthread.h"

#include <QImage>

QLchChromaLightnessDiagrammWorkerThread::QLchChromaLightnessDiagrammWorkerThread(QObject *parent) : QThread(parent)
{
}

QLchChromaLightnessDiagrammWorkerThread::~QLchChromaLightnessDiagrammWorkerThread()
{
}

bool QLchChromaLightnessDiagrammWorkerThread::abort() const
{
    return m_abort;
}

void QLchChromaLightnessDiagrammWorkerThread::run()
{
    QImage tempImage = QImage(size(), QImage::Format_RGB32);
    tempImage.fill(palette().color(QPalette::Window)); // initialize with window background: palette().color(QPalette::Window)
    int maxHeight = size().height() - 1; // TODO make sure this is > 0 (because of division by 0)
    int maxWidth = size().width() - 1; // TODO make sure this is > 0 (because of division by 0)
    double lightness;
    double chroma;
    ColorSpace::Lch lch;
    ColorSpace::Rgb rgb;
    QColor tempColor;
    for (int y = 0; y <= maxHeight; ++y) {
        lightness = y * static_cast<double>(100) / maxHeight; // floating point division thanks to 100 which is a "double"
        for (int x = 0; x <= maxWidth; ++x) {
            chroma = x * internalMaximumChroma / maxWidth; // floating point division thanks to internalMaximumChroma which is "double"
            lch = ColorSpace::Lch(lightness, chroma, internalHue);
            lch.To<ColorSpace::Rgb>(&rgb);
            tempColor = QColor::fromRgbF(rgb.r / 255, rgb.g / 255, rgb.b / 255);
            if (!tempColor.isValid()) { // if color is out-of-gammut
                // We have chroma on the x axis and lightness on the y axis. We are drawing the pixmap line per line,
                // so we go for given lightness from low chroma to high chroma. Because of the nature of the diagram,
                // if once in a line we have an out-of-gammut value, all other pixels that are more at the right will
                // be out-of-gammut also. So we optimize our code and break here.
                break;
            }
            tempImage.setPixelColor(x, maxHeight - y, tempColor);
        }
    }
}

void QLchChromaLightnessDiagrammWorkerThread::setAbort(bool abort)
{
    if (m_abort == abort) {
        return;
    }

    m_abort = abort;
    emit abortChanged(m_abort);
}

bool QLchChromaLightnessDiagrammWorkerThread::restart() const
{
    return m_restart;
}

void QLchChromaLightnessDiagrammWorkerThread::setRestart(bool restart)
{
    if (m_restart == restart) {
        return;
    }

    m_restart = restart;
    emit restartChanged(m_restart);
}
