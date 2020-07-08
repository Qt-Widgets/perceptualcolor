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
#include <QSignalSpy>
#include <QTest>
#include <qtestcase.h>
#include "PerceptualColor/colordialog.h"

class TestColorDialog : public QObject
{
    Q_OBJECT

private:
    QPointer<PerceptualColor::ColorDialog> m_perceptualDialog;
    QPointer<PerceptualColor::ColorDialog> m_perceptualDialog2;
    QPointer<QColorDialog> m_qDialog;
    QPointer<QColorDialog> m_qDialog2;
    const QColor emptyConstructor = QColor(1, 2, 3);
    QColor m_color;

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
        if (m_perceptualDialog2) {
            delete m_perceptualDialog2;
        }
        if (m_qDialog) {
            delete m_qDialog;
        }
        if (m_qDialog2) {
            delete m_qDialog2;
        }
    };

    static void voidMessageHandler(QtMsgType, const QMessageLogContext &, const QString &) {
        // dummy message handler that does not print messages
    }
    
    void helperProvideQColors() {
        qInstallMessageHandler(voidMessageHandler); // supress warning for generating invalid QColor

        QTest::addColumn<QColor>("color");
        QTest::newRow("RGB 1 2 3") << QColor(1, 2, 3);
        QTest::newRow("RGBA 1 2 3 4") << QColor(1, 2, 3, 4);
        QTest::newRow("RGB 1 2 300") << QColor(1, 2, 300);
        QTest::newRow("RGBA 1 2 300 4") << QColor(1, 2, 300, 4);

        QTest::newRow("RGB 0.1 0.2 0.3") << QColor::fromRgbF(0.1, 0.2, 0.3);
        QTest::newRow("RGBA 0.1 0.2 0.3 0.4") << QColor::fromRgbF(0.1, 0.2, 0.3, 0.4);
        QTest::newRow("RGB 0.1 6.2 0.300") << QColor::fromRgbF(0.1, 6.2, 0.300);
        QTest::newRow("RGBA 0.1 6.2 0.300 0.4") << QColor::fromRgbF(0.1, 6.2, 0.300, 0.4);

        QTest::newRow("CMYK 1 2 3 4") << QColor::fromCmyk(1, 2, 3, 4);
        QTest::newRow("CMYK 1 2 3 4 5") << QColor::fromCmyk(1, 2, 3, 4, 5);
        QTest::newRow("CMYK 1 2 300 4") << QColor::fromCmyk(1, 2, 300, 4);
        QTest::newRow("CMYK 1 2 300 4 5") << QColor::fromCmyk(1, 2, 300, 4, 5);
        QTest::newRow("CMYK 0.1 0.2 0.300 0.4") << QColor::fromCmykF(0.1, 0.2, 0.300, 0.4);
        QTest::newRow("CMYK 0.1 0.2 0.300 0.4 0.6495217645") << QColor::fromCmykF(0.1, 0.2, 0.300, 0.4, 0.6495217645);
        QTest::newRow("CMYK 0.1 6.2 0.300 0.4") << QColor::fromCmykF(0.1, 6.2, 0.300, 0.4);
        QTest::newRow("CMYK 0.1 6.2 0.300 0.4 0.6495217645") << QColor::fromCmykF(0.1, 6.2, 0.300, 0.4, 0.6495217645);
        
        QTest::newRow("HSL 2 3 4") << QColor::fromHsl(2, 3, 4);
        QTest::newRow("HSL 2 3 4 5") << QColor::fromHsl(2, 3, 4, 5);
        QTest::newRow("HSL 2 300 4") << QColor::fromHsl(2, 300, 4);
        QTest::newRow("HSL 2 300 4 5") << QColor::fromHsl(2, 300, 4, 5);
        QTest::newRow("HSL 0.2 0.300 0.4") << QColor::fromHslF(0.2, 0.300, 0.4);
        QTest::newRow("HSL 0.2 0.300 0.4 0.6495217645") << QColor::fromHslF(0.2, 0.300, 0.4, 0.6495217645);
        QTest::newRow("HSL 6.2 0.300 0.4") << QColor::fromHslF(6.2, 0.300, 0.4);
        QTest::newRow("HSL 6.2 0.300 0.4 0.6495217645") << QColor::fromHslF(6.2, 0.300, 0.4, 0.6495217645);
        
        QTest::newRow("HSV 2 3 4") << QColor::fromHsv(2, 3, 4);
        QTest::newRow("HSV 2 3 4 5") << QColor::fromHsv(2, 3, 4, 5);
        QTest::newRow("HSV 2 300 4") << QColor::fromHsv(2, 300, 4);
        QTest::newRow("HSV 2 300 4 5") << QColor::fromHsv(2, 300, 4, 5);
        QTest::newRow("HSV 0.2 0.300 0.4") << QColor::fromHsvF(0.2, 0.300, 0.4);
        QTest::newRow("HSV 0.2 0.300 0.4 0.6495217645") << QColor::fromHsvF(0.2, 0.300, 0.4, 0.6495217645);
        QTest::newRow("HSV 6.2 0.300 0.4") << QColor::fromHsvF(6.2, 0.300, 0.4);
        QTest::newRow("HSV 6.2 0.300 0.4 0.6495217645") << QColor::fromHsvF(6.2, 0.300, 0.4, 0.6495217645);

        QTest::newRow("invalid") << QColor();

        qInstallMessageHandler(0); // supress warning for generating invalid QColor anymore
    }

    void helperCompareDialog(PerceptualColor::ColorDialog *m_perceptualDialog, QColorDialog *m_qDialog) {
        // Compare the state of m_perceptualDialog (actual) to m_qDialog (expected)
        QCOMPARE(
            m_perceptualDialog->selectedColor().name(),
            m_qDialog->selectedColor().name()
        );
        QCOMPARE(
            m_perceptualDialog->selectedColor().alpha(),
            m_qDialog->selectedColor().alpha()
        );
        QCOMPARE(
            m_perceptualDialog->selectedColor().spec(),
            m_qDialog->selectedColor().spec()
        );
        QCOMPARE(
            m_perceptualDialog->currentColor().name(),
            m_qDialog->currentColor().name()
        );
        QCOMPARE(
            m_perceptualDialog->currentColor().alpha(),
            m_qDialog->currentColor().alpha()
        );
        QCOMPARE(
            m_perceptualDialog->currentColor().spec(),
            m_qDialog->currentColor().spec()
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
        QCOMPARE(
            m_perceptualDialog->parent(),
            m_qDialog->parent()
        );
        QCOMPARE(
            m_perceptualDialog->parentWidget(),
            m_qDialog->parentWidget()
        );
    }

    void testConstructorQWidget() {
        // Test the constructor ColorDialog(QWidget * parent = nullptr)
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        QWidget *tempWidget = new QWidget();
        m_perceptualDialog2 = new PerceptualColor::ColorDialog(tempWidget);
        // Test post-condition: currentColor() is Qt::white
        QCOMPARE(m_perceptualDialog->currentColor(), Qt::white);
        QCOMPARE(m_perceptualDialog2->currentColor(), Qt::white);
        QCOMPARE(m_perceptualDialog2->parentWidget(), tempWidget);
        QCOMPARE(m_perceptualDialog2->parent(), tempWidget);
    }
        
    void testConstructorQWidgetConformance() {
        // Test the constructor ColorDialog(QWidget * parent = nullptr)
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        QWidget *tempWidget = new QWidget();
        m_perceptualDialog2 = new PerceptualColor::ColorDialog(tempWidget);
        // Test if this behaviour is conformant to QColorDialog
        m_qDialog = new QColorDialog();
        m_qDialog2 = new QColorDialog(tempWidget);
        helperCompareDialog(m_perceptualDialog, m_qDialog);
        helperCompareDialog(m_perceptualDialog2, m_qDialog2);
        delete tempWidget;
    }
    
    void testConstructorQColorQWidget_data() {
        helperProvideQColors();
    }

    void testConstructorQColorQWidget() {
        QFETCH(QColor, color);
        QColor colorOpaque;
        if (color.isValid()) {
            colorOpaque = color.toRgb();
            colorOpaque.setAlpha(255);
        } else {
            colorOpaque = Qt::black;
        }

        // Test the constructor ColorDialog(QWidget * parent = nullptr)
        m_perceptualDialog = new PerceptualColor::ColorDialog(color);
        QWidget *tempWidget = new QWidget();
        m_perceptualDialog2 = new PerceptualColor::ColorDialog(color, tempWidget);
        // Test post-condition: currentColor() is color
        QCOMPARE(m_perceptualDialog->currentColor().name(), colorOpaque.name());
        QCOMPARE(m_perceptualDialog->currentColor().alpha(), colorOpaque.alpha());
        QCOMPARE(m_perceptualDialog->currentColor().spec(), colorOpaque.spec());
        QCOMPARE(m_perceptualDialog2->currentColor().name(), colorOpaque.name());
        QCOMPARE(m_perceptualDialog2->currentColor().alpha(), colorOpaque.alpha());
        QCOMPARE(m_perceptualDialog2->currentColor().spec(), colorOpaque.spec());
        QCOMPARE(m_perceptualDialog2->parentWidget(), tempWidget);
        QCOMPARE(m_perceptualDialog2->parent(), tempWidget);
    }
    
    void testConstructorQColorQWidgetConformance_data() {
        helperProvideQColors();
    }

    void testConstructorQColorQWidgetConformance() {
        QFETCH(QColor, color);

        // Test the constructor ColorDialog(QWidget * parent = nullptr)
        m_perceptualDialog = new PerceptualColor::ColorDialog(color);
        QWidget *tempWidget = new QWidget();
        m_perceptualDialog2 = new PerceptualColor::ColorDialog(color, tempWidget);
        // Test if this behaviour is conformant to QColorDialog
        m_qDialog = new QColorDialog(color);
        m_qDialog2 = new QColorDialog(color, tempWidget);
        helperCompareDialog(m_perceptualDialog, m_qDialog);
        helperCompareDialog(m_perceptualDialog2, m_qDialog2);
        delete tempWidget;
    }

    void testSetOptionAndTestOption() {
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
/* TODO This is overkill:
        colorList.append(QPair<QString, QColor>("RGB 1 2 3", QColor(1, 2, 3)));
        colorList.append(QPair<QString, QColor>("RGBA 1 2 3 4", QColor(1, 2, 3, 4)));
        colorList.append(QPair<QString, QColor>("RGB 1 2 300", QColor(1, 2, 300)));
        colorList.append(QPair<QString, QColor>("RGBA 1 2 300 4", QColor(1, 2, 300, 4)));

        colorList.append(QPair<QString, QColor>("RGB 0.1 0.2 0.3", QColor::fromRgbF(0.1, 0.2, 0.3)));
        colorList.append(QPair<QString, QColor>("RGBA 0.1 0.2 0.3 0.4", QColor::fromRgbF(0.1, 0.2, 0.3, 0.4)));
        colorList.append(QPair<QString, QColor>("RGB 0.1 6.2 0.300", QColor::fromRgbF(0.1, 6.2, 0.300)));
        colorList.append(QPair<QString, QColor>("RGBA 0.1 6.2 0.300 0.4", QColor::fromRgbF(0.1, 6.2, 0.300, 0.4)));

        colorList.append(QPair<QString, QColor>("CMYK 1 2 3 4", QColor::fromCmyk(1, 2, 3, 4)));
        colorList.append(QPair<QString, QColor>("CMYK 1 2 3 4 5", QColor::fromCmyk(1, 2, 3, 4, 5)));
        colorList.append(QPair<QString, QColor>("CMYK 1 2 300 4", QColor::fromCmyk(1, 2, 300, 4)));
        colorList.append(QPair<QString, QColor>("CMYK 1 2 300 4 5", QColor::fromCmyk(1, 2, 300, 4, 5)));
        colorList.append(QPair<QString, QColor>("CMYK 0.1 0.2 0.300 0.4", QColor::fromCmykF(0.1, 0.2, 0.300, 0.4)));
        colorList.append(QPair<QString, QColor>("CMYK 0.1 0.2 0.300 0.4 0.6495217645", QColor::fromCmykF(0.1, 0.2, 0.300, 0.4, 0.6495217645)));
        colorList.append(QPair<QString, QColor>("CMYK 0.1 6.2 0.300 0.4", QColor::fromCmykF(0.1, 6.2, 0.300, 0.4)));
        colorList.append(QPair<QString, QColor>("CMYK 0.1 6.2 0.300 0.4 0.6495217645", QColor::fromCmykF(0.1, 6.2, 0.300, 0.4, 0.6495217645)));
        
        colorList.append(QPair<QString, QColor>("HSL 2 3 4", QColor::fromHsl(2, 3, 4)));
        colorList.append(QPair<QString, QColor>("HSL 2 3 4 5", QColor::fromHsl(2, 3, 4, 5)));
        colorList.append(QPair<QString, QColor>("HSL 2 300 4", QColor::fromHsl(2, 300, 4)));
        colorList.append(QPair<QString, QColor>("HSL 2 300 4 5", QColor::fromHsl(2, 300, 4, 5)));
        colorList.append(QPair<QString, QColor>("HSL 0.2 0.300 0.4", QColor::fromHslF(0.2, 0.300, 0.4)));
        colorList.append(QPair<QString, QColor>("HSL 0.2 0.300 0.4 0.6495217645", QColor::fromHslF(0.2, 0.300, 0.4, 0.6495217645)));
        colorList.append(QPair<QString, QColor>("HSL 6.2 0.300 0.4", QColor::fromHslF(6.2, 0.300, 0.4)));
        colorList.append(QPair<QString, QColor>("HSL 6.2 0.300 0.4 0.6495217645", QColor::fromHslF(6.2, 0.300, 0.4, 0.6495217645)));
        
        colorList.append(QPair<QString, QColor>("HSV 2 3 4", QColor::fromHsv(2, 3, 4)));
        colorList.append(QPair<QString, QColor>("HSV 2 3 4 5", QColor::fromHsv(2, 3, 4, 5)));
        colorList.append(QPair<QString, QColor>("HSV 2 300 4", QColor::fromHsv(2, 300, 4)));
        colorList.append(QPair<QString, QColor>("HSV 2 300 4 5", QColor::fromHsv(2, 300, 4, 5)));
        colorList.append(QPair<QString, QColor>("HSV 0.2 0.300 0.4", QColor::fromHsvF(0.2, 0.300, 0.4)));
        colorList.append(QPair<QString, QColor>("HSV 0.2 0.300 0.4 0.6495217645", QColor::fromHsvF(0.2, 0.300, 0.4, 0.6495217645)));
        colorList.append(QPair<QString, QColor>("HSV 6.2 0.300 0.4", QColor::fromHsvF(6.2, 0.300, 0.4)));
        colorList.append(QPair<QString, QColor>("HSV 6.2 0.300 0.4 0.6495217645", QColor::fromHsvF(6.2, 0.300, 0.4, 0.6495217645)));
*/
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
    
    void testConformanceWithQColorDialog() {
        return; // TODO remove this line
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
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, showAlphaChannel);
        m_qDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, showAlphaChannel);
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, noButtons);
        m_qDialog->setOption(QColorDialog::ColorDialogOption::NoButtons, noButtons);
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->show();
        m_qDialog->show();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->setCurrentColor(secondColor);
        m_qDialog->setCurrentColor(secondColor);
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->show();
        m_qDialog->show();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        QTest::keyClick(m_perceptualDialog, Qt::Key_Return);
        QTest::keyClick(m_qDialog, Qt::Key_Return);
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->show();
        m_qDialog->show();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->setCurrentColor(secondColor);
        m_qDialog->setCurrentColor(secondColor);
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->hide();
        m_qDialog->hide();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        m_perceptualDialog->show();
        m_qDialog->show();
        helperCompareDialog(m_perceptualDialog, m_qDialog);

        QTest::keyClick(m_perceptualDialog, Qt::Key_Escape);
        QTest::keyClick(m_qDialog, Qt::Key_Escape);
        helperCompareDialog(m_perceptualDialog, m_qDialog);
    };

    void testColorSelectedSignal() {
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        m_perceptualDialog->show();
        m_qDialog = new QColorDialog();
        m_qDialog->show();
        QSignalSpy spyPerceptualDialog(m_perceptualDialog, &PerceptualColor::ColorDialog::colorSelected);
        QSignalSpy spyQDialog(m_qDialog, &QColorDialog::colorSelected);
        QTest::keyClick(m_perceptualDialog, Qt::Key_Return);
        QTest::keyClick(m_qDialog, Qt::Key_Return);
        QCOMPARE(spyPerceptualDialog.count(), 1);
        QCOMPARE(spyPerceptualDialog.count(), spyQDialog.count());
        m_perceptualDialog->show();
        m_qDialog->show();
        QTest::keyClick(m_perceptualDialog, Qt::Key_Escape);
        QTest::keyClick(m_qDialog, Qt::Key_Escape);
        QCOMPARE(spyPerceptualDialog.count(), 1);
        QCOMPARE(spyPerceptualDialog.count(), spyQDialog.count());
        m_perceptualDialog->show();
        m_qDialog->show();
        QTest::keyClick(m_perceptualDialog, Qt::Key_Return);
        QTest::keyClick(m_qDialog, Qt::Key_Return);
        QCOMPARE(spyPerceptualDialog.count(), 2);
        QCOMPARE(spyPerceptualDialog.count(), spyQDialog.count());
    }

    void testPropertyConformance_data() {
        QTest::addColumn<QString>("propertyName");
        QMetaObject referenceClass = QColorDialog::staticMetaObject;
        for (int i = 0; i < referenceClass.propertyCount(); ++i) {
            QTest::newRow(referenceClass.property(i).name()) << referenceClass.property(i).name();
        }
    }

    void testPropertyConformance() {
        QFETCH(QString, propertyName);
        QMetaObject testClass = PerceptualColor::ColorDialog::staticMetaObject;
        QMetaObject referenceClass = QColorDialog::staticMetaObject;
        int testClassIndex = testClass.indexOfProperty(propertyName.toLatin1());
        int referenceClassIndex = referenceClass.indexOfProperty(propertyName.toLatin1());
        QMetaProperty referenceClassProperty = referenceClass.property(referenceClassIndex);
        QString message;
        message = QString("Test if property \"")
            + referenceClassProperty.name()
            + "\" of class \""
            + referenceClass.className()
            + "\" is also available in \""
            + testClass.className()
            + "\".";
        QVERIFY2(testClassIndex >= 0, message.toLatin1());
        QMetaProperty testClassProperty = testClass.property(testClassIndex);
        QCOMPARE(testClassProperty.hasNotifySignal(), referenceClassProperty.hasNotifySignal());
        QCOMPARE(testClassProperty.isConstant(), referenceClassProperty.isConstant());
        QCOMPARE(testClassProperty.isDesignable(), referenceClassProperty.isDesignable());
        QCOMPARE(testClassProperty.isEnumType(), referenceClassProperty.isEnumType());
        if (referenceClassProperty.isEnumType()) {
            QCOMPARE(testClassProperty.enumerator().enumName(), referenceClassProperty.enumerator().enumName());
            QCOMPARE(testClassProperty.enumerator().isFlag(), referenceClassProperty.enumerator().isFlag());
            QCOMPARE(testClassProperty.enumerator().isScoped(), referenceClassProperty.enumerator().isScoped());
            QCOMPARE(testClassProperty.enumerator().isValid(), referenceClassProperty.enumerator().isValid());
            QCOMPARE(testClassProperty.enumerator().keyCount(), referenceClassProperty.enumerator().keyCount());
            QCOMPARE(testClassProperty.enumerator().name(), referenceClassProperty.enumerator().name());
            QCOMPARE(testClassProperty.enumerator().scope(), referenceClassProperty.enumerator().scope());
        }
        QCOMPARE(testClassProperty.isFinal(), referenceClassProperty.isFinal());
        QCOMPARE(testClassProperty.isFlagType(), referenceClassProperty.isFlagType());
        QCOMPARE(testClassProperty.isReadable(), referenceClassProperty.isReadable());
        QCOMPARE(testClassProperty.isResettable(), referenceClassProperty.isResettable());
        QCOMPARE(testClassProperty.isScriptable(), referenceClassProperty.isScriptable());
        QCOMPARE(testClassProperty.isStored(), referenceClassProperty.isStored());
        QCOMPARE(testClassProperty.isUser(), referenceClassProperty.isUser());
        QCOMPARE(testClassProperty.isValid(), referenceClassProperty.isValid());
        QCOMPARE(testClassProperty.isWritable(), referenceClassProperty.isWritable());
        QCOMPARE(testClassProperty.isWritable(), referenceClassProperty.isWritable());
        QCOMPARE(testClassProperty.name(), referenceClassProperty.name());
        QCOMPARE(testClassProperty.notifySignal().methodSignature(), referenceClassProperty.notifySignal().methodSignature());
        QCOMPARE(testClassProperty.notifySignal().methodType(), referenceClassProperty.notifySignal().methodType());
        QCOMPARE(testClassProperty.notifySignal().name(), referenceClassProperty.notifySignal().name());
        QCOMPARE(testClassProperty.notifySignal().parameterCount(), referenceClassProperty.notifySignal().parameterCount());
        QCOMPARE(testClassProperty.notifySignal().parameterNames(), referenceClassProperty.notifySignal().parameterNames());
        QCOMPARE(testClassProperty.notifySignal().parameterTypes(), referenceClassProperty.notifySignal().parameterTypes());
        QCOMPARE(testClassProperty.notifySignal().returnType(), referenceClassProperty.notifySignal().returnType());
        QCOMPARE(testClassProperty.notifySignal().revision(), referenceClassProperty.notifySignal().revision());
        QCOMPARE(testClassProperty.notifySignal().tag(), referenceClassProperty.notifySignal().tag());
        QCOMPARE(testClassProperty.notifySignal().typeName(), referenceClassProperty.notifySignal().typeName());
        QCOMPARE(testClassProperty.type(), referenceClassProperty.type());
        QCOMPARE(testClassProperty.typeName(), referenceClassProperty.typeName());
        QCOMPARE(testClassProperty.userType(), referenceClassProperty.userType());
    }

    void testMethodConformance_data() {
        QTest::addColumn<QByteArray>("methodSignature");
        QTest::addColumn<int>("referenceClassIndex");
        QMetaObject referenceClass = QColorDialog::staticMetaObject;
        for (int i = 0; i < referenceClass.methodCount(); ++i) {
            if (referenceClass.method(i).access() != QMetaMethod::Private) {
                // Exclude private methods from conformance check
                QTest::newRow(referenceClass.method(i).name())
                    << QMetaObject::normalizedSignature(referenceClass.method(i).methodSignature())
                    << i;
            }
        }
    }

    void testMethodConformance() {
        // We do only check QMetaObject::method() and
        // not QMetaObject::constructor because QColorDialog
        // does not provide its constructors to the
        // meta-object system.
        QFETCH(QByteArray, methodSignature);
        QFETCH(int, referenceClassIndex);
        QMetaObject testClass = PerceptualColor::ColorDialog::staticMetaObject;
        QMetaObject referenceClass = QColorDialog::staticMetaObject;
        int testClassIndex = testClass.indexOfMethod(methodSignature);
        QMetaMethod referenceClassMethod = referenceClass.method(referenceClassIndex);
        QString message;
        message = QString("Test if method \"")
            + referenceClassMethod.methodSignature()
            + "\" of class \""
            + referenceClass.className()
            + "\" is also available in \""
            + testClass.className()
            + "\".";
        QVERIFY2(testClassIndex >= 0, message.toLatin1());
        QMetaMethod testClassMethod = testClass.method(testClassIndex);
        QCOMPARE(testClassMethod.access(), referenceClassMethod.access());
        QCOMPARE(testClassMethod.isValid(), referenceClassMethod.isValid());
        QCOMPARE(testClassMethod.methodSignature(), referenceClassMethod.methodSignature());
        QCOMPARE(testClassMethod.methodType(), referenceClassMethod.methodType());
        QCOMPARE(testClassMethod.name(), referenceClassMethod.name());
        QCOMPARE(testClassMethod.parameterCount(), referenceClassMethod.parameterCount());
        QCOMPARE(testClassMethod.parameterNames(), referenceClassMethod.parameterNames());
        QCOMPARE(testClassMethod.parameterTypes(), referenceClassMethod.parameterTypes());
        QCOMPARE(testClassMethod.returnType(), referenceClassMethod.returnType());
        QCOMPARE(testClassMethod.revision(), referenceClassMethod.revision());
        QCOMPARE(testClassMethod.tag(), referenceClassMethod.tag());
        QCOMPARE(testClassMethod.typeName(), referenceClassMethod.typeName());
    }

    void testRttiConformance() {
        QMetaObject testClass = PerceptualColor::ColorDialog::staticMetaObject;
        QMetaObject referenceClass = QColorDialog::staticMetaObject;
        QString message;
        message = QString("Test that \"") + testClass.className() + "\" inherits from \"" + referenceClass.className() + "\"'s superclass.";
        QVERIFY2(
            testClass.inherits(referenceClass.superClass()),
            message.toLatin1()
        );
    }
    
    void testCurrentColorChangedSignal() {
        m_perceptualDialog = new PerceptualColor::ColorDialog();
        m_qDialog = new QColorDialog();
        m_perceptualDialog->show();
        m_qDialog->show();
        QSignalSpy spyPerceptualDialog(m_perceptualDialog, &PerceptualColor::ColorDialog::currentColorChanged);
        QSignalSpy spyQDialog(m_qDialog, &QColorDialog::currentColorChanged);
        
        // Test that a simple “return key” click by the user does not call this signal
        QTest::keyClick(m_perceptualDialog, Qt::Key_Return);
        QTest::keyClick(m_qDialog, Qt::Key_Return);
        QCOMPARE(spyPerceptualDialog.count(), 0);
        QCOMPARE(spyPerceptualDialog.count(), spyQDialog.count());
        
        // A different color should call the signal
        m_perceptualDialog->setCurrentColor(QColor(1, 2, 3));
        m_qDialog->setCurrentColor(QColor(1, 2, 3));
        QCOMPARE(spyPerceptualDialog.count(), 1);
        QCOMPARE(spyPerceptualDialog.count(), spyQDialog.count());
        
        // The same color again should not call again the signal
        m_perceptualDialog->setCurrentColor(QColor(1, 2, 3));
        m_qDialog->setCurrentColor(QColor(1, 2, 3));
        QCOMPARE(spyPerceptualDialog.count(), 1);
        QCOMPARE(spyPerceptualDialog.count(), spyQDialog.count());
        
        // The same RGB values, but defined in another color model, should not
        // emit a signal eihter.
        m_perceptualDialog->setCurrentColor(QColor(1, 2, 3).toHsl());
        m_qDialog->setCurrentColor(QColor(1, 2, 3).toHsl());
        QCOMPARE(spyPerceptualDialog.count(), 1);
        QCOMPARE(spyPerceptualDialog.count(), spyQDialog.count());
        
        // Changing QColorDialog::ColorDialogOption::ShowAlphaChannel should
        // not emit a signal either
        m_perceptualDialog->setOption(
            QColorDialog::ColorDialogOption::ShowAlphaChannel,
            false
        );
        m_qDialog->setOption(
            QColorDialog::ColorDialogOption::ShowAlphaChannel,
            false
        );
        QCOMPARE(spyPerceptualDialog.count(), 1);
        QCOMPARE(spyPerceptualDialog.count(), spyQDialog.count());
    }

    void testCurrentColorProperty_data() {
        helperProvideQColors();
    }
    
    void testCurrentColorProperty() {
        QFETCH(QColor, color);
        QColor correctedColor;
        if (color.isValid()) {
            correctedColor = color.toRgb();
        } else {
            correctedColor = Qt::black;
        }
        QColor opaqueColor = correctedColor;
        opaqueColor.setAlpha(255);
        
        m_perceptualDialog = new PerceptualColor::ColorDialog;
        m_qDialog = new QColorDialog;

        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
        m_qDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
        m_perceptualDialog->setCurrentColor(color);
        m_qDialog->setCurrentColor(color);
        // Test conformance (but only integer precision)
        QCOMPARE(m_perceptualDialog->currentColor().rgb(), m_qDialog->currentColor().rgb());
        QCOMPARE(m_perceptualDialog->currentColor().alpha(), m_qDialog->currentColor().alpha());
        QCOMPARE(
            static_cast<int>(m_perceptualDialog->currentColor().spec()),
            static_cast<int>(m_qDialog->currentColor().spec())
        );
        // Test postcondition (but only integer precision)
        QCOMPARE(m_perceptualDialog->currentColor().rgb(), correctedColor.rgb());
        QCOMPARE(m_perceptualDialog->currentColor().alpha(), correctedColor.alpha());
        QCOMPARE(
            static_cast<int>(m_perceptualDialog->currentColor().spec()),
            static_cast<int>(correctedColor.spec())
        );

        // Test that changing QColorDialog::ColorDialogOption::ShowAlphaChannel
        // alone does not change the currentColor property
        m_perceptualDialog->setOption(
            QColorDialog::ColorDialogOption::ShowAlphaChannel, 
            false
        );
        m_qDialog->setOption(
            QColorDialog::ColorDialogOption::ShowAlphaChannel, 
            false
        );
        // Test conformance (but only integer precision)
        QCOMPARE(m_perceptualDialog->currentColor().rgb(), m_qDialog->currentColor().rgb());
        QCOMPARE(m_perceptualDialog->currentColor().alpha(), m_qDialog->currentColor().alpha());
        QCOMPARE(
            static_cast<int>(m_perceptualDialog->currentColor().spec()),
            static_cast<int>(m_qDialog->currentColor().spec())
        );
        // Test postcondition (but only integer precision)
        QCOMPARE(m_perceptualDialog->currentColor().rgb(), correctedColor.rgb());
        QCOMPARE(m_perceptualDialog->currentColor().alpha(), correctedColor.alpha());
        QCOMPARE(
            static_cast<int>(m_perceptualDialog->currentColor().spec()),
            static_cast<int>(correctedColor.spec())
        );

        m_perceptualDialog->setOption(
            QColorDialog::ColorDialogOption::ShowAlphaChannel, 
            false
        );
        m_qDialog->setOption(
            QColorDialog::ColorDialogOption::ShowAlphaChannel, 
            false
        );
        m_perceptualDialog->setCurrentColor(color);
        m_qDialog->setCurrentColor(color);
        // Test conformance (but only integer precision)
        QCOMPARE(m_perceptualDialog->currentColor().rgb(), m_qDialog->currentColor().rgb());
        QCOMPARE(m_perceptualDialog->currentColor().alpha(), m_qDialog->currentColor().alpha());
        QCOMPARE(
            static_cast<int>(m_perceptualDialog->currentColor().spec()),
            static_cast<int>(m_qDialog->currentColor().spec())
        );
        // Test postcondition (but only integer precision)
        QCOMPARE(m_perceptualDialog->currentColor().rgb(), opaqueColor.rgb());
        QCOMPARE(m_perceptualDialog->currentColor().alpha(), opaqueColor.alpha());
        QCOMPARE(
            static_cast<int>(m_perceptualDialog->currentColor().spec()),
            static_cast<int>(opaqueColor.spec())
        );
        

        // Test that changing QColorDialog::ColorDialogOption::ShowAlphaChannel alone does not change the currentColor property
        m_perceptualDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
        m_qDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
        // Test conformance (but only integer precision)
        QCOMPARE(m_perceptualDialog->currentColor().rgb(), m_qDialog->currentColor().rgb());
        QCOMPARE(m_perceptualDialog->currentColor().alpha(), m_qDialog->currentColor().alpha());
        QCOMPARE(
            static_cast<int>(m_perceptualDialog->currentColor().spec()),
            static_cast<int>(m_qDialog->currentColor().spec())
        );
        // Test postcondition (but only integer precision)
        QCOMPARE(m_perceptualDialog->currentColor().rgb(), opaqueColor.rgb());
        QCOMPARE(m_perceptualDialog->currentColor().alpha(), opaqueColor.alpha());
        QCOMPARE(
            static_cast<int>(m_perceptualDialog->currentColor().spec()),
            static_cast<int>(opaqueColor.spec())
        );
    }
    
    void helperReceiveSignals(QColor color) {
        m_color = color;
    }

    void testOpen() {
        // Test our reference (QColorDialog)
        m_color = Qt::black;
        m_qDialog = new QColorDialog;
        m_qDialog->setCurrentColor(Qt::white);
        m_qDialog->open(this, SLOT(helperReceiveSignals(QColor)));
        m_qDialog->setCurrentColor(Qt::red);
        // Changing the current color does not emit the signal
        QCOMPARE(m_color, Qt::black);
        QTest::keyClick(m_qDialog, Qt::Key_Return);
        // Return key really emits a signal
        QCOMPARE(m_color, Qt::red);
        m_qDialog->show();
        m_qDialog->setCurrentColor(Qt::green);
        QTest::keyClick(m_qDialog, Qt::Key_Return);
        // The signal is really disconncted after the dialog has been closed.
        QCOMPARE(m_color, Qt::red);
        
        // Now test if PerceptualColor::ColorDialog does the same thing as our reference
        m_color = Qt::black;
        m_perceptualDialog = new PerceptualColor::ColorDialog;
        m_perceptualDialog->setCurrentColor(Qt::white);
        m_perceptualDialog->open(this, SLOT(helperReceiveSignals(QColor)));
        m_perceptualDialog->setCurrentColor(Qt::red);
        // Changing the current color does not emit the signal
        QCOMPARE(m_color, Qt::black);
        QTest::keyClick(m_perceptualDialog, Qt::Key_Return);
        // Return key really emits a signal
        QCOMPARE(m_color, Qt::red);
        m_perceptualDialog->show();
        m_perceptualDialog->setCurrentColor(Qt::green);
        QTest::keyClick(m_perceptualDialog, Qt::Key_Return);
        // The signal is really disconncted after the dialog has been closed.
        QCOMPARE(m_color, Qt::red);
    }

};

QTEST_MAIN(TestColorDialog);
#include "testcolordialog.moc" // necessary because we do not use a header file
