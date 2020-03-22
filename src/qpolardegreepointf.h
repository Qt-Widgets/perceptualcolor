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

#ifndef QPOLARDEGREEPOINTF_H
#define QPOLARDEGREEPOINTF_H

#include <QtGlobal>
#include <QPointF>

/**
 * @todo write docs
 */
class QPolarDegreePointF
{
public:
    /**
     * Default constructor
     */
    QPolarDegreePointF();

    /**
     * Copy constructor
     *
     * @param other TODO
     */
    QPolarDegreePointF(const QPolarDegreePointF& other) = default;
    QPolarDegreePointF(const qreal newRadial, const qreal newAngleDegree);
    QPolarDegreePointF(const QPointF newPoint);

    /**
     * Destructor
     */
    virtual ~QPolarDegreePointF();

    /**
     * Assignment operator
     *
     * @param other TODO
     * @return TODO
     */
    QPolarDegreePointF& operator=(const QPolarDegreePointF& other) = default;

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator==(const QPolarDegreePointF& other) const;

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator!=(const QPolarDegreePointF& other) const;

    qreal radial() const;
    qreal angleDegree() const;
    void setPolarDegreeCoordinates(const qreal radial, const qreal angleDegree);
    static qreal normalizedAngleDegree(const qreal angle);
    QPointF toQPointF() const;
private:
    qreal m_radial;
    qreal m_angleDegree;
};

#endif // QPOLARDEGREEPOINTF_H
