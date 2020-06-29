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

#include <QObject>
#include <QPointer>
#include <QTest>
#include "PerceptualColor/colordialog.h"

class TestColorDialog : public QObject
{
    Q_OBJECT

private:
    QPointer<PerceptualColor::ColorDialog> m_perceptualDialog;
    QPointer<QColorDialog> m_qDialog;
    const QColor emptyConstructor = QColor(1, 2, 3);

private Q_SLOTS:
    void initTestCase() {
        // Called before the first testfunction is executed
    };
    void cleanupTestCase() {
        // Called after the last testfunction was executed
    };

    void init() {
        // Called before each testfunction is executed
    };

    void cleanup() {
        // Called after every testfunction
        if (m_perceptualDialog) {
            delete m_perceptualDialog;
        }
        if (m_qDialog) {
            delete m_qDialog;
        }
    };

    void testSetOptionAndTestOption() {
        bool cache;
        
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        // Test if the option changes as expected
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), true);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, false);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), false);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), true);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, false);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), false);
        delete m_perceptualDialog;
        
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        // Test if the option changes as expected
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, false);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), false);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), true);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, false);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), false);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), true);
        delete m_perceptualDialog;
        
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        // Test if the option changes as expected
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, true);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::NoButtons), true);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, false);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::NoButtons), false);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, true);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::NoButtons), true);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, false);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::NoButtons), false);
        delete m_perceptualDialog;
        
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        // Test if the option changes as expected
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, false);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::NoButtons), false);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, true);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::NoButtons), true);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, false);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::NoButtons), false);
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, true);
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::NoButtons), true);
        delete m_perceptualDialog;
        
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        // define an option
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
        // change some other option
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, true);
        // test if first option is still unchanged
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), true);
        delete m_perceptualDialog;
        
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        // define an option
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, false);
        // change some other option
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, true);
        // test if first option is still unchanged
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), false);
        delete m_perceptualDialog;
        
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        // define an option
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
        // change some other option
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, false);
        // test if first option is still unchanged
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), true);
        delete m_perceptualDialog;
        
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        // define an option
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, false);
        // change some other option
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, false);
        // test if first option is still unchanged
        QCOMPARE(m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel), false);
        delete m_perceptualDialog;
    }

    void testConformanceWithQColorDialog_data() {
        QTest::addColumn<QColor>("initialColor");
        QTest::addColumn<QColor>("secondColor");
        QTest::addColumn<bool>("showAlphaChannel");
        QTest::addColumn<bool>("noButtons");

        QList<QPair<QString, QColor>> colorList;
        colorList.append(QPair<QString, QColor>("redOpaque", QColor(255, 0, 0)));
        colorList.append(QPair<QString, QColor>("greenOpaque", QColor(0, 255, 0)));
        colorList.append(QPair<QString, QColor>("redHalf", QColor(255, 0, 0, 128)));
        colorList.append(QPair<QString, QColor>("greenHalf", QColor(0, 255, 0, 128)));
        colorList.append(QPair<QString, QColor>("redTransparent", QColor(255, 0, 0, 255)));
        colorList.append(QPair<QString, QColor>("greenTransparent", QColor(0, 255, 0, 255)));
        colorList.append(QPair<QString, QColor>("qtTransparent", QColor(Qt::transparent)));
        colorList.append(QPair<QString, QColor>("invalid", QColor()));
        colorList.append(QPair<QString, QColor>("empty", emptyConstructor)); // special placeholder for empty constructor
        for (int i = 0; i < colorList.size(); ++i) {
            for (int j = 0; j < colorList.size(); ++j) {
                QTest::newRow((QString(colorList.at(i).first) + QString("/") + QString(colorList.at(j).first) + QString("/ShowAlphaChannel/NoButtons")).toLatin1())
                    << colorList.at(i).second
                    << colorList.at(j).second
                    << true
                    << true;
                QTest::newRow((QString(colorList.at(i).first) + QString("/") + QString(colorList.at(j).first) + QString("/ShowAlphaChannel")).toLatin1())
                    << colorList.at(i).second
                    << colorList.at(j).second
                    << true
                    << false;
                QTest::newRow((QString(colorList.at(i).first) + QString("/") + QString(colorList.at(j).first) + QString("/NoButtons")).toLatin1())
                    << colorList.at(i).second
                    << colorList.at(j).second
                    << false
                    << true;
                QTest::newRow((QString(colorList.at(i).first) + QString("/") + QString(colorList.at(j).first) + QString("")).toLatin1())
                    << colorList.at(i).second
                    << colorList.at(j).second
                    << false
                    << false;
            }
        }
    }

    void helperDialogCompare(PerceptualColor::ColorDialog *m_perceptualDialog, QColorDialog *m_qDialog) {
        // Compare the state of m_perceptualDialog (actual) to m_qDialog (expected)
        QCOMPARE(
            m_perceptualDialog->selectedColor(),
            m_qDialog->selectedColor()
        );
        QCOMPARE(
            m_perceptualDialog->currentColor(),
            m_qDialog->currentColor()
        );
        QCOMPARE(
            m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::NoButtons),
            m_qDialog->testOption(QColorDialog::ColorDialogOption::NoButtons)
        );
        QCOMPARE(
            m_perceptualDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel),
            m_qDialog->testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel)
        );
        QCOMPARE(
            m_perceptualDialog->options().testFlag(QColorDialog::ColorDialogOption::NoButtons),
            m_qDialog->options().testFlag(QColorDialog::ColorDialogOption::NoButtons)
        );
        QCOMPARE(
            m_perceptualDialog->options().testFlag(QColorDialog::ColorDialogOption::ShowAlphaChannel),
            m_qDialog->options().testFlag(QColorDialog::ColorDialogOption::ShowAlphaChannel)
        );
        QCOMPARE(
            m_perceptualDialog->isVisible(),
            m_qDialog->isVisible()
        );
        QCOMPARE(
            m_perceptualDialog->isModal(),
            m_qDialog->isModal()
        );
        QCOMPARE(
            m_perceptualDialog->result(),
            m_qDialog->result()
        );
    }
    
    void testConformanceWithQColorDialog() {
        QFETCH(QColor, initialColor);
        QFETCH(QColor, secondColor);
        QFETCH(bool, showAlphaChannel);
        QFETCH(bool, noButtons);

        if (initialColor == emptyConstructor) { // special placeholder for empty constructor
            m_perceptualDialog = new PerceptualColor::ColorDialog();
            m_qDialog = new QColorDialog();
        } else {
            m_perceptualDialog = new PerceptualColor::ColorDialog(initialColor);
            m_qDialog = new QColorDialog(initialColor);
        }
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, showAlphaChannel);
        m_qDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, showAlphaChannel);
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, noButtons);
        m_qDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, noButtons);
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->show();
        m_qDialog->show();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->setCurrentColor(secondColor);
        m_qDialog->setCurrentColor(secondColor);
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->show();
        m_qDialog->show();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        QTest::keyClick(m_perceptualDialog, Qt::Key_Return);
        QTest::keyClick(m_qDialog, Qt::Key_Return);
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->show();
        m_qDialog->show();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->setCurrentColor(secondColor);
        m_qDialog->setCurrentColor(secondColor);
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->show();
        m_qDialog->show();
        helperDialogCompare(m_perceptualDialog, m_qDialog);

        QTest::keyClick(m_perceptualDialog, Qt::Key_Escape);
        QTest::keyClick(m_qDialog, Qt::Key_Escape);
        helperDialogCompare(m_perceptualDialog, m_qDialog);
    };
};

QTEST_MAIN(TestColorDialog);
#include "testcolordialog.moc" // necessary because we do not use a header file
