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

#ifndef QLCHCHROMALIGHTNESSDIAGRAMMWORKERTHREAD_H
#define QLCHCHROMALIGHTNESSDIAGRAMMWORKERTHREAD_H

#include <QObject>
#include <QThread>

/**
 * @todo write docs
 */
class QLchChromaLightnessDiagrammWorkerThread : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool abort READ abort WRITE setAbort NOTIFY abortChanged)
    Q_PROPERTY(bool restart READ restart WRITE setRestart NOTIFY restartChanged)

public:
    /**
     * Default constructor
     */
    QLchChromaLightnessDiagrammWorkerThread(QObject parent);

    /**
     * Destructor
     */
    virtual ~QLchChromaLightnessDiagrammWorkerThread();

    /**
     * @return the abort
     */
    bool abort() const;

    /**
     * @return the restart
     */
    bool restart() const;

public Q_SLOTS:
    /**
     * Sets the abort.
     *
     * @param abort the new abort
     */
    void setAbort(bool abort);

    /**
     * Sets the restart.
     *
     * @param restart the new restart
     */
    void setRestart(bool restart);

Q_SIGNALS:
    void abortChanged(bool abort);

    void restartChanged(bool restart);

protected:
    virtual void run();

private:
    Q_DISABLE_COPY(QLchChromaLightnessDiagrammWorkerThread);
    bool m_abort;
    bool m_restart;
};

#endif // QLCHCHROMALIGHTNESSDIAGRAMMWORKERTHREAD_H
