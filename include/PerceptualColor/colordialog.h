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

#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include "PerceptualColor/alphaselector.h"
#include "PerceptualColor/colorpatch.h"
#include "PerceptualColor/chromahuediagram.h"
#include "PerceptualColor/fullcolordescription.h"
#include "PerceptualColor/gradientselector.h"
#include "PerceptualColor/wheelcolorpicker.h"

#include <QByteArray>
#include <QColorDialog>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLayout>
#include <QLineEdit>
#include <QObject>
#include <QPointer>
#include <QVBoxLayout>

namespace PerceptualColor {

/** @brief Dialog for specifying colors perceptually.
 * 
 * Provides a dialog to choose an RGB color like QColorDialog, but with color
 * pepresentation on screen based on the more intuitive the perceptual LCh color
 * model.
 * 
 * The API of this class is fully source-compatible to the API of QColorDialog
 * and the API behaves exactly as for QColorDialog (if not, it's a bug, please
 * report it), with the following exceptions:
 * - As this dialog does not provide functionality for custom colors and
 *   standard color, the corresponding static functions of QColorDialog are
 *   not available in this class.
 * - The option <tt>QColorDialog::ColorDialogOption::DontUseNativeDialog</tt>
 *   will always remain <tt>false</tt> (even if set explicitly), because it's
 *   just the point of this library to provide an own, non-native dialog.
 */
class ColorDialog : public QDialog
{
    Q_OBJECT

    /** @brief Currently selected color
     * 
     * This property is provides as an RGB value.
     * 
     * @sa currentColor()
     * @sa setCurrentColor()
     * @sa currentColorChanged()
     * @sa m_currentOpaqueFullColor()
     * @sa m_alpha
     */
    Q_PROPERTY(QColor currentColor READ currentColor WRITE setCurrentColor NOTIFY currentColorChanged USER true)

public:
    explicit ColorDialog(QWidget *parent = nullptr);
    explicit ColorDialog(const QColor &initial, QWidget *parent = nullptr);
    virtual ~ColorDialog() override;
    QColor currentColor() const;
    QColorDialog::ColorDialogOptions options() const;
    void setOption(QColorDialog::ColorDialogOption option, bool on = true);
    void setOptions(QColorDialog::ColorDialogOptions options);
    bool testOption(QColorDialog::ColorDialogOption option) const;
    virtual void setVisible(bool visible) override;
    QColor selectedColor() const;
    using QDialog::open; // Make sure the base classes “open“ function won't be overwritten
    void open(QObject *receiver, const char *member);
    static QColor getColor(
        const QColor &initial = Qt::white,
        QWidget *parent = nullptr,
        const QString &title = QString(),
        QColorDialog::ColorDialogOptions options = QColorDialog::ColorDialogOptions()
    );

Q_SIGNALS:
    /** Notify signal for currentColor() property. */
    void currentColorChanged(const QColor &color);
    void colorSelected(const QColor &color);

public Q_SLOTS:
    void setCurrentColor(const QColor &color);

protected:
    virtual void done(int result) override;
/* TODO Static Public Members
QColor	customColor(int index)
int	customCount()
void	setCustomColor(int index, QColor color)
void	setStandardColor(int index, QColor color)
QColor	standardColor(int index)
 */
private:
    Q_DISABLE_COPY(ColorDialog)

    /** @brief Holds the current (opaque) color without alpha information
     * 
     * The information about the alpha channel is stored within m_alphaSelector
     * 
     * @sa currentColor() */
    FullColorDescription m_currentOpaqueFullColor;
    QColor m_selectedColor;
    ColorPatch *m_colorPatch;
    QLineEdit *m_hlcLineEdit;
    QLineEdit *m_rgbLineEdit;
    QDoubleSpinBox *m_rgbBlueSpinbox;
    RgbColorSpace *m_rgbColorSpace;
    QDoubleSpinBox *m_rgbGreenSpinbox;
    QDoubleSpinBox *m_rgbRedSpinbox;
    QDoubleSpinBox *m_hsvHueSpinbox;
    QDoubleSpinBox *m_hsvSaturationSpinbox;
    QDoubleSpinBox *m_hsvValueSpinbox;
    WheelColorPicker *m_wheelColorPicker;
    ChromaHueDiagram *m_chromaHueDiagram;
    GradientSelector *m_lchLightnessSelector;
    AlphaSelector *m_alphaSelector;
    QLabel *m_alphaSelectorLabel;
    QDialogButtonBox  *m_buttonBox;
    bool m_isColorChangeInProgress = false;
    QColorDialog::ColorDialogOptions m_options;
    QPointer<QObject> m_receiverToBeDisconnected;
    QByteArray m_memberToBeDisconnected;

    void initialize();
    QWidget* initializeRgbPage();
    QWidget* initializeLabPage();

private Q_SLOTS:
    void updateOpaqueColorWidgets(const PerceptualColor::FullColorDescription &color);
    void readHsvNumericValues();
    void readRgbNumericValues();
    void readRgbHexValues();
    void readLightnessValue();
    void setCurrentOpaqueFullColor(const PerceptualColor::FullColorDescription &color);
    void handleFocusChange(QWidget *old, QWidget *now);
    void readHlcNumericValues();
    void setCurrentOpaqueQColor(const QColor &color);
};

}

#endif // COLORDIALOG_H
