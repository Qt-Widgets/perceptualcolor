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

#include "qpolardegreepointf.h"
#include <math.h>
#include <QtMath>

QPolarDegreePointF::QPolarDegreePointF()
{
    m_radial = 0;
    m_angleDegree = 0;
}

QPolarDegreePointF::QPolarDegreePointF(const qreal newRadial, const qreal newAngleDegree)
{
    if (newRadial == 0) {
        m_radial = 0;
        m_angleDegree = 0; // The angle is meaningless when the radial is 0. By convention we define him to 0.
        return;
    } else {
        if (newRadial < 0) {
            m_radial = newRadial * (-1);
            m_angleDegree = normalizedAngleDegree(newAngleDegree + 180);
        } else {
            m_radial = newRadial;
            m_angleDegree = normalizedAngleDegree(newAngleDegree);
        }
    }
}

QPolarDegreePointF::QPolarDegreePointF(const QPointF newPoint)
{
    m_radial = sqrt( pow(newPoint.x(), 2) + pow(newPoint.y(), 2) );
    if (m_radial == 0) {
        m_angleDegree = 0;
        return;
    }
    if (newPoint.y() >= 0) {
        m_angleDegree = qRadiansToDegrees(acos(newPoint.x() / m_radial));
    } else {
        m_angleDegree = qRadiansToDegrees(2 * (M_PI) - acos(newPoint.x() / m_radial)); // M_PI is defined by QtMath (also on platforms that don't support it natively)
    }
}

QPolarDegreePointF::~QPolarDegreePointF()
{
}

bool QPolarDegreePointF::operator==(const QPolarDegreePointF& other) const
{
    return ( (m_radial == other.m_radial) && (m_angleDegree == other.m_angleDegree) );
}

bool QPolarDegreePointF::operator!=(const QPolarDegreePointF& other) const
{
    return ( (m_radial != other.m_radial) || (m_angleDegree != other.m_angleDegree) );
}

qreal QPolarDegreePointF::radial() const
{
    return m_radial;
}

qreal QPolarDegreePointF::angleDegree() const
{
    return m_angleDegree;
}

qreal QPolarDegreePointF::normalizedAngleDegree(const qreal angle)
{
    qreal temp = fmod(angle, 360);
    if (temp < 0) {
        return temp + 360;
    } else {
        return temp;
    }
}

QPointF QPolarDegreePointF::toQPointF() const
{
    return QPointF(
        m_radial * cos(qDegreesToRadians(m_angleDegree)),
        m_radial * sin(qDegreesToRadians(m_angleDegree))
    );
}
