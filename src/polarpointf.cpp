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

// Own header
#include "PerceptualColor/polarpointf.h"

// Other includes
#include <QtMath>

namespace PerceptualColor {

/** @brief Constructor
 * 
 * Constructs an object with radial() = 0 and angleDegree() = 0 */
PolarPointF::PolarPointF()
{
    m_radial = 0;
    m_angleDegree = 0;
}

/** @brief Constructor
 * 
 * Constructs an object with the given radial and angle values. The
 * values get normalized, see the general class description for details.
 * 
 * @param newRadial the radial value
 * @param newAngleDegree the angle mesured in degree */
PolarPointF::PolarPointF(const qreal newRadial, const qreal newAngleDegree)
{
    if (newRadial < 0) {
        m_radial = newRadial * (-1);
        m_angleDegree = normalizedAngleDegree(newAngleDegree + 180);
    } else {
        m_radial = newRadial;
        m_angleDegree = normalizedAngleDegree(newAngleDegree);
    }
}

/** @brief Constructor
 * 
 * Constructs an object converting from the given cartesian coordinates.
 * 
 * If the cartesian coordinates are (0, 0) than the angle (which is
 * meaningless for a radial of 0) is set to 0°.
 * 
 * @param cartesianCoordiantes the cartesian coordinates */
PolarPointF::PolarPointF(const QPointF cartesianCoordiantes)
{
    m_radial = sqrt(
        pow(cartesianCoordiantes.x(), 2) + pow(cartesianCoordiantes.y(), 2)
    );
    if (m_radial == 0) {
        m_angleDegree = 0;
        return;
    }
    if (cartesianCoordiantes.y() >= 0) {
        m_angleDegree = qRadiansToDegrees(
            acos(cartesianCoordiantes.x() / m_radial)
        );
    } else {
        // M_PI is defined by QtMath (also on platforms that don't
        // support it natively)
        m_angleDegree = qRadiansToDegrees(
            2 * (M_PI) - acos(cartesianCoordiantes.x() / m_radial)
        );
    }
}

/** @brief Equal operator
 * 
 * Equal means, both values reference to the same point in the coordinates
 * space. So <tt>(radial: 0, angleDegree: 50)</tt> is considered equal to
 * <tt>(radial: 0, angleDegree: 80)</tt> because the angleDegree is
 * meaningless if the radial is 0.
 * 
 * @param other the polar coordinates to compare with
 * @returns @c true if equal, @c false otherwise */
bool PolarPointF::operator==(const PolarPointF other) const
{
    return (
        // radial has to be identical
        (m_radial == other.m_radial) &&
        // angle has to be identical (except when radial is 0, because then angle is meaningless)
        ( (m_angleDegree == other.m_angleDegree) || (m_radial == 0) )
    );
}

/** @brief Unequal operator
 * 
 * Inverse behaviour to operator==()
 * 
 * @param other the polar coordinates to compare with
 * @returns @c true if unequal, @c false otherwise */
bool PolarPointF::operator!=(const PolarPointF other) const
{
    return !(*this == other);
}

/** @brief Normalized radial
 *
 * @returns the normalized radial value, guaranteed to be ≥ 0. */
qreal PolarPointF::radial() const
{
    return m_radial;
}

/** @brief Normalized angle
 *
 * @returns the normalized angle value (mesured in degree), guaranteed to
 * be 0° ≤ value < 360° */
qreal PolarPointF::angleDegree() const
{
    return m_angleDegree;
}

/** @brief Normalizes an angle.
 * 
 * \li 0° gets 0°
 * \li 359.9° gets 359.9°
 * \li 360° gets 0°
 * \li 361.2° gets 1.2°
 * \li 720° gets 0°
 * \li -1° gets 359°
 * \li -1.3° gets 358.7°
 * 
 * @param angle an angle (mesured in degree)
 * @returns the angle, normalized to the range 0° ≤ value < 360° */
qreal PolarPointF::normalizedAngleDegree(const qreal angleDegree)
{
    qreal temp = fmod(angleDegree, 360);
    if (temp < 0) {
        temp += 360;
    }
    return temp;
}

/** @brief Convert to cartesion coordinates
 * 
 * @returns the corresponding cartesian coordinates */
QPointF PolarPointF::toCartesian() const
{
    return QPointF(
        m_radial * cos(qDegreesToRadians(m_angleDegree)),
        m_radial * sin(qDegreesToRadians(m_angleDegree))
    );
}

/** @brief Adds QDebug() support for this data type. */
QDebug operator<<(QDebug dbg, const PerceptualColor::PolarPointF polarpointf)
{
    dbg.nospace()
        << "PolarPointF(radial: "
        << polarpointf.radial()
        << ", angleDegree: "
        << polarpointf.angleDegree()
        << "°)";
    return dbg.maybeSpace();
}

} // namespace PerceptualColor
