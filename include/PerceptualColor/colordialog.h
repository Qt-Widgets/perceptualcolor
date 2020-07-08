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
#include <QLineEdit>
#include <QObject>
#include <QPointer>

namespace PerceptualColor {

/** @brief Dialog for specifying colors perceptually.
 * 
 * The color dialog's function is to allow users to choose colors intuitively.
 * For example, you might use this in a drawing program to allow the
 * user to set the brush color. It is a (almost) source-compatible replacement
 * for QColorDialog. At difference to QColorDialog, this dialog's
 * graphical components are perceptually uniform and intuitive.
 * It's internally based on the LCh color model, which does
 * reflect the human perceptuan much better than RGB or its
 * transforms like HSV. At the same time, this dialog does not require the
 * user itself to know anything about LCh at all, because the graphical
 * representations tend to be intuitive.
 * 
 * Just as with QColorDialog, the static functions provide modal color
 * dialogs. The static getColor() function shows the dialog, and allows
 * the user to specify a color:
 * @code
 * QColor myColor = PerceptualColor::ColorDialog::getColor();
 * @endcode
 * This function can also be used to let
 * users choose a color with a level of transparency: pass the
 * QColorDialog::ColorDialogOption::ShowAlphaChannel option as an
 * additional argument:
 * @code
 * QColor myColor = PerceptualColor::ColorDialog::getColor(
 *     Qt::green,      // current color at widget startup
 *     0,              // parent widget
 *     "Window title", // window title
 *     QColorDialog::ColorDialogOption::ShowAlphaChannel
 * );
 * @endcode
 * 
 * @note The API of this class is fully source-compatible to the API of
 * QColorDialog and the API behaves exactly as for QColorDialog (if not,
 * it's a bug; please report it), with the following exceptions:
 * - As this dialog does not provide functionality for custom colors and
 *   standard color, the corresponding static functions of QColorDialog are
 *   not available in this class.
 * - The option <tt>QColorDialog::ColorDialogOption::DontUseNativeDialog</tt>
 *   will always remain <tt>false</tt> (even if set explicitly), because it's
 *   just the point of this library to provide an own, non-native dialog.
 * - Calling setCurrentColor() with colors that
 *   are @em not <tt>QColor::Spec::Rgb</tt> will lead to an automatic
 *   conversion like QColorDialog does, but at difference to QColorDialog, the
 *   PerceptualColor::ColorDialog has more precision, therefor the resulting
 *   currentColor() might be slightly different. The same is true
 *   for <tt>QColor::Spec::Rgb</tt> types with floating point precision:
 *   While QColorDialog would round to full
 *   integers, PerceptualColor::ColorDialog preserves the floating point
 *   precision. */
class ColorDialog : public QDialog
{
    Q_OBJECT

    /** @brief Currently selected color in the dialog
     * 
     * @invariant This property is provided as an RGB value.
     * <tt>QColor::isValid()</tt> is always @c true and
     * <tt>QColor::spec()</tt> is always <tt>QColor::Spec::Rgb</tt>.
     * 
     * @invariant The signal currentColorChanged() is emitted always and only
     * when the value of this property changes.
     * 
     * @note The setter setCurrentColor() does not accept all QColor values.
     * See its documentation for details.
     * 
     * @sa READ currentColor()
     * @sa WRITE setCurrentColor()
     * @sa NOTIFY currentColorChanged()
     * @sa m_currentOpaqueColor */
    Q_PROPERTY(
        QColor currentColor
        READ currentColor
        WRITE setCurrentColor
        NOTIFY currentColorChanged
    )

    /** @brief Various options that affect the look and feel of the dialog
     * 
     * These are the same settings as for QColorDialog. They are also of the
     * same type: @c QColorDialog::ColorDialogOption
     * 
     * | Option              | Default value | Description
     * | :------------------ | :------------ | :----------
     * | ShowAlphaChannel    | false         | Allow the user to select the alpha component of a color.
     * | NoButtons           | false         | Don't display OK and Cancel buttons. (Useful for "live dialogs".)
     * | DontUseNativeDialog | true          | Use Qt's standard color dialog instead of the operating system native color dialog.
     * 
     *   @note The option
     *   <tt>QColorDialog::ColorDialogOption::DontUseNativeDialog</tt> will
     *   always remain <tt>false</tt> (even if set explicitly), because it's
     *   just the point of this library to provide an own, non-native
     *   dialog. */
    Q_PROPERTY(
        QColorDialog::ColorDialogOptions options
        READ options
        WRITE setOptions
    )
        
public:
    explicit ColorDialog(QWidget *parent = nullptr);
    explicit ColorDialog(const QColor &initial, QWidget *parent = nullptr);
    virtual ~ColorDialog() override;
    /** @brief Getter for property currentColor()
     *  @returns the property currentColor() */
    QColor currentColor() const;
    static QColor getColor(
        const QColor &initial = Qt::white,
        QWidget *parent = nullptr,
        const QString &title = QString(),
        QColorDialog::ColorDialogOptions options = QColorDialog::ColorDialogOptions()
    );
    using QDialog::open; // Make sure not override base class's “open“ function
    void open(QObject *receiver, const char *member);
    /** @brief Getter for property options()
     * @returns the current options */
    QColorDialog::ColorDialogOptions options() const;
    QColor selectedColor() const;
    void setOption(QColorDialog::ColorDialogOption option, bool on = true);
    void setOptions(QColorDialog::ColorDialogOptions options);
    virtual void setVisible(bool visible) override;
    bool testOption(QColorDialog::ColorDialogOption option) const;

public Q_SLOTS:
    void setCurrentColor(const QColor &color);

Q_SIGNALS:
    /** @brief This signal is emitted just after the user has clicked OK to
     * select a color to use.
     *  @param color the chosen color */
    void colorSelected(const QColor &color);
    /** @brief Notify signal for property currentColor().
     * 
     * This signal is emitted whenever the current color changes in the
     * dialog.
     * @param color The current color */
    void currentColorChanged(const QColor &color);

protected:
    virtual void done(int result) override;

private:
    Q_DISABLE_COPY(ColorDialog)

    /** @brief Pointer to the GradientSelector for alpha. */
    AlphaSelector *m_alphaSelector;
    /** @brief Pointer to the QLabel for m_alphaSelector().
     * 
     * We store this in a
     * pointer to allow toggle the visibility later. */
    QLabel *m_alphaSelectorLabel;
    /** @brief Pointer to the QButtonBox of this dialog.
     * 
     * We store this in a pointer
     * to allow toggle the visibility later. */
    QDialogButtonBox  *m_buttonBox;
    /** @brief Pointer to the ChromaLightnessDiagram. */
    ChromaHueDiagram *m_chromaHueDiagram;
    /** @brief Pointer to the ColorPatch widget. */
    ColorPatch *m_colorPatch;
    /** @brief Holds the current color without alpha information
     * 
     * @note The alpha information within this data member is meaningless.
     * Ignore it. The information about the alpha channel is actually stored
     * within m_alphaSelector().
     * 
     * @sa currentColor() */
    FullColorDescription m_currentOpaqueColor;
    /** @brief Pointer to the GradientSelector for LCh lightness. */
    GradientSelector *m_lchLightnessSelector;
    /** @brief Pointer to the QLineEdit that represents the HLC value. */
    QLineEdit *m_hlcLineEdit;
    /** @brief Pointer to the QSpinbox for HSV hue. */
    QDoubleSpinBox *m_hsvHueSpinbox;
    /** @brief Pointer to the QSpinbox for HSV saturation. */
    QDoubleSpinBox *m_hsvSaturationSpinbox;
    /** @brief Pointer to the QSpinbox for HSV value. */
    QDoubleSpinBox *m_hsvValueSpinbox;
    /** @brief Holds the receiver slot (if any) to be disconnected
     *  automatically after closing the dialog.
     * 
     * Its value is only meaningfull if
     * m_receiverToBeDisconnected() is not null.
     * @sa m_receiverToBeDisconnected
     * @sa open() */
    QByteArray m_memberToBeDisconnected;
    /** @brief Holds wether currently a color change is ongoing, or not.
     * 
     * Used to avoid infinite recursions when updating the different widgets
     * within this dialog.
     * @sa setCurrentOpaqueFullColor() */
    bool m_isColorChangeInProgress = false;
    /** @brief Holds the receiver object (if any) to be disconnected
     *  automatically after closing the dialog.
     * 
     * @sa m_memberToBeDisconnected
     * @sa open() */
    QPointer<QObject> m_receiverToBeDisconnected;
    /** @brief Internal storage for property options() */
    QColorDialog::ColorDialogOptions m_options;
    /** @brief Pointer to the QSpinbox for RGB blue. */
    QDoubleSpinBox *m_rgbBlueSpinbox;
    /** @brief Pointer to the RgbColorSpace object. */
    RgbColorSpace *m_rgbColorSpace;
    /** @brief Pointer to the QSpinbox for RGB green. */
    QDoubleSpinBox *m_rgbGreenSpinbox;
    /** @brief Pointer to the QLineEdit that represents the hexadecimal
     *  RGB value. */
    QLineEdit *m_rgbLineEdit;
    /** @brief Pointer to the QSpinbox for RGB red. */
    QDoubleSpinBox *m_rgbRedSpinbox;
    /** @brief Internal storage for selectedColor(). */
    QColor m_selectedColor;
    /** @brief Pointer to the WheelColorPicker widget. */
    WheelColorPicker *m_wheelColorPicker;

    void initialize();
    QWidget* initializeNumericPage();

private Q_SLOTS:
    void handleFocusChange(QWidget *old, QWidget *now);
    void readHlcNumericValues();
    void readHsvNumericValues();
    void readLightnessValue();
    void readRgbHexValues();
    void readRgbNumericValues();
    void setCurrentOpaqueColor(
        const PerceptualColor::FullColorDescription &color
    );
    void setCurrentOpaqueQColor(const QColor &color);
};

}

#endif // COLORDIALOG_H
