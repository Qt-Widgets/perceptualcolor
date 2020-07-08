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
#include "PerceptualColor/colordialog.h"

#include <QApplication>
#include <QFormLayout>
#include <QGroupBox>
#include <QRegularExpressionValidator>
#include <QTabWidget>
#include <QVBoxLayout>

namespace PerceptualColor {

/** @brief Constructor
 * 
 *  @param parent pointer to the parent widget, if any
 *  @post The currentColor() property is set to Qt::white. */
ColorDialog::ColorDialog(QWidget *parent) : QDialog(parent)
{
    initialize();
    setCurrentColor(QColor(Qt::white)); // That's what QColorDialog also does
}

/** @brief Constructor
 * 
 *  @param initial the initially chosen color of the
 *  dialog
 *  @param parent pointer to the parent widget, if any
 *  @post The object is constructed and setCurrentColor() is called
 *  with @em initial. See setCurrentColor() for the modifications that
 *  will be applied before setting the current color. Especially, as
 *  this dialog is constructed by default without alpha support, the
 *  alpha channel of @em initial is ignored and a fully opaque color is
 *  used. */
ColorDialog::ColorDialog(const QColor &initial, QWidget *parent) : QDialog(parent)
{
    initialize();
    setCurrentColor(initial);
}

/** @brief Destructor */
ColorDialog::~ColorDialog()
{
    // All the layouts and widgets used here are automatically child widgets
    // of this dialog widget. Therefor they are deleted automatically.
    // Also m_rgbColorSpace is of type RgbColorSpace(), which inherits from
    // QObject, and is a child of this dialog widget, does not need to be
    // deleted manually.
}

// No documentation here (documentation of properties and its getters are in the header)
QColor ColorDialog::currentColor() const
{
    // TODO Conformance QColorDialog when later the alpha option is changed. (Should alpha option change also trigger a currentColorChanged() signal?
    QColor temp;
    temp = m_currentOpaqueColor.toRgbQColor();
    temp.setAlphaF(m_alphaSelector->alpha());
    return temp;
}

/** @brief Setter for currentColor() property.
 * 
 * @param color the new color
 * \post The property currentColor() is adapted as follows:
 * - If @em color is not valid, <tt>Qt::black</tt> is used instead.
 * - If <em>color</em>'s <tt>QColor::Spec</tt> is @em not
 *   <tt>QColor::Spec::Rgb</tt> then it will be converted silently
 *   to <tt>QColor::Spec::Rgb</tt>
 * - The RGB part of currentColor() will be the RGB part of <tt>color</tt>.
 * - The alpha channel of currentColor() will be the alpha channel
 *   of <tt>color</tt> if at the moment of the function call
 *   the <tt>QColorDialog::ColorDialogOption::ShowAlphaChannel</tt> option is
 *   set. It will be fully opaque otherwise. */
// TODO unit test for emited signal when this property is changed. And if alpha option changes?
void ColorDialog::setCurrentColor(const QColor& color)
{
    QColor temp;
    if (color.isValid()) {
        temp = color;
        // QColorDialog would instead call QColor.rgb() which
        // rounds to 8 bit per channel.
    } else {
        // For invalid colors same behaviour as QColorDialog
        temp = QColor(Qt::black);
    }
    if (testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel)) {
        m_alphaSelector->setAlpha(color.alphaF());
    } else {
        m_alphaSelector->setAlpha(1);
    }
    setCurrentOpaqueColor(FullColorDescription(m_rgbColorSpace, temp));
}

/** @brief Opens the dialog and connects its colorSelected() signal to the
 * slot specified by receiver and member.
 * 
 * The signal will be disconnected from the slot when the dialog is closed.
 * 
 * Example:
 * @code
 *  PerceptualColor::ColorDialog m_dialog = new PerceptualColor::ColorDialog;
 *  m_dialog->open(this, SLOT(mySlot(QColor)));
 * @endcode */
void ColorDialog::open(QObject *receiver, const char *member)
{
    connect(this, SIGNAL(colorSelected(QColor)), receiver, member); // TODO would this work with &PerceptualColor::ColorDialog::colorSelected
    m_receiverToBeDisconnected = receiver;
    m_memberToBeDisconnected = member;
    QDialog::open();
}

/** @brief Convenience version of setCurrentOpaqueColor(), accepting QColor
 * @param color the new color. Expected to be in RGB color space (RGB, HSV etc.) */
void ColorDialog::setCurrentOpaqueQColor(const QColor& color)
{
    setCurrentOpaqueColor(FullColorDescription(m_rgbColorSpace, color));
}

/** @brief Updates m_currentOpaqueColor() and all affected widgets.
 * 
 * This function ignores the alpha component!
 * @param color the new color
 * @post If color is invalid, nothing happens. If this function is called
 * recursively, nothing happens. Else m_currentOpaqueColor is updated,
 * and the corresponding widgets are updated.
 * @note Recursive functions calls are ignored. This is useful, because you
 * can connect signals from various widgets to this slot without having to
 * worry about infinite recursions. */
void ColorDialog::setCurrentOpaqueColor(const FullColorDescription& color)
{
    if (m_isColorChangeInProgress || (!color.isValid()) || (color == m_currentOpaqueColor)) {
        // Nothing to do!
        return;
    }

    // If we have really work to do, block recursive calls of this function
    m_isColorChangeInProgress = true;

    // Same currentColor() for later comparision
    // Using currentColor() makes sure correct alpha treatment!
    QColor oldQColor = currentColor();

    // Update m_currentOpaqueColor
    m_currentOpaqueColor = color;

    // Update all the widgets for opaque color…
    QColor tempRgbQColor = color.toRgbQColor();
    tempRgbQColor.setAlpha(255);
    m_rgbRedSpinbox->setValue(tempRgbQColor.redF() * 255);
    m_rgbGreenSpinbox->setValue(tempRgbQColor.greenF() * 255);
    m_rgbBlueSpinbox->setValue(tempRgbQColor.blueF() * 255);
    m_hsvHueSpinbox->setValue(color.toHsvQColor().hsvHueF() * 360);
    m_hsvSaturationSpinbox->setValue(color.toHsvQColor().hsvSaturationF() * 255);
    m_hsvValueSpinbox->setValue(color.toHsvQColor().valueF() * 255);
    m_colorPatch->setColor(tempRgbQColor);
    m_hlcLineEdit->setText(
        QString(QStringLiteral(u"%1 %2 %3"))
            .arg(color.toLch().h, 0, 'f', 0)
            .arg(color.toLch().L, 0, 'f', 0)
            .arg(color.toLch().C, 0, 'f', 0)
    );
    m_rgbLineEdit->setText(tempRgbQColor.name());
    m_lchLightnessSelector->setFraction(color.toLch().L / static_cast<qreal>(100));
    m_chromaHueDiagram->setColor(color);
    m_wheelColorPicker->setCurrentColor(m_currentOpaqueColor);
    m_alphaSelector->setColor(m_currentOpaqueColor);

    // Emit signal currentColorChanged() only if necessary
    if (currentColor() != oldQColor) {
        Q_EMIT currentColorChanged(currentColor());
    }

    // End of this function. Unblock resursive function calls before returning.
    m_isColorChangeInProgress = false;
}

///////// ##################################

void ColorDialog::readLightnessValue()
{
    cmsCIELCh lch = m_currentOpaqueColor.toLch();
    lch.L = m_lchLightnessSelector->fraction() * 100;
    setCurrentOpaqueColor(
        FullColorDescription(m_rgbColorSpace, lch, FullColorDescription::outOfGamutBehaviour::sacrifyChroma)
    );
}

void ColorDialog::readHsvNumericValues()
{
    setCurrentOpaqueQColor(
        QColor::fromHsvF(
            m_hsvHueSpinbox->value() / static_cast<qreal>(360),
            m_hsvSaturationSpinbox->value() / static_cast<qreal>(255),
            m_hsvValueSpinbox->value() / static_cast<qreal>(255)
        )
    );
}

void ColorDialog::readRgbNumericValues()
{
    setCurrentOpaqueQColor(
        QColor::fromRgbF(
            m_rgbRedSpinbox->value() / static_cast<qreal>(255),
            m_rgbGreenSpinbox->value() / static_cast<qreal>(255),
            m_rgbBlueSpinbox->value() / static_cast<qreal>(255)
        )
    );
}

// TODO Default dialog size should depend on screen size:
// - on smaller screens it should never exceed the screen boundary.
// - on larger screens is should be big to allow optimal use of available screen size

// TODO the graphical selector widgets (the wheel and the ChromaHueDiagram() ) should be
// horizontally and vertically centered.

void ColorDialog::readRgbHexValues()
{
    QString temp = m_rgbLineEdit->text();
    if (!temp.startsWith('#')) {
        temp = QString('#') + temp;
    }
    QColor rgb;
    rgb.setNamedColor(temp);
    if (rgb.isValid()) {
        setCurrentOpaqueQColor(rgb);
    }
    // Return to the finally considered value.
    m_rgbLineEdit->setText(m_currentOpaqueColor.toRgbQColor().name());
}

/** Basic initialization. Code that is shared between the various
 * overloaded constructors.
 */
void ColorDialog::initialize()
{
    // initialize color space
    m_rgbColorSpace = new RgbColorSpace(this);

    // initialize the options
    m_options = QColorDialog::ColorDialogOption::DontUseNativeDialog;

    // create the graphical selectors
    m_wheelColorPicker = new WheelColorPicker(m_rgbColorSpace);
    m_currentOpaqueColor = m_wheelColorPicker->currentColor();
    m_lchLightnessSelector = new GradientSelector(m_rgbColorSpace);
    m_lchLightnessSelector->setColors(
        FullColorDescription(m_rgbColorSpace, Qt::black),
        FullColorDescription(m_rgbColorSpace, Qt::white)
    );
    m_chromaHueDiagram = new ChromaHueDiagram(m_rgbColorSpace);
    QHBoxLayout *tempLightnesFirstLayout = new QHBoxLayout();
    tempLightnesFirstLayout->addWidget(m_lchLightnessSelector);
    tempLightnesFirstLayout->addWidget(m_chromaHueDiagram);
    QWidget *tempWidget = new QWidget();
    tempWidget->setLayout(tempLightnesFirstLayout);
    QTabWidget *tempGraphicalTabWidget = new QTabWidget;
    // TODO the second tab has GradientSelector + ChromaHueDiagram, and the ChromaHueDiagram
    // is smaller than WheelColorPicker on the first tab. That's because of the GradientSelector
    // of the second tab, who takes away space compared to the first tab. The good solution
    // would be to have a larger tab widget, that allows for ChromaHueDiagram to get at
    // least the size from its sizeHint().
    tempGraphicalTabWidget->addTab(m_wheelColorPicker, tr("&Hue first")); // TODO Use an icon instead of text
    tempGraphicalTabWidget->addTab(tempWidget, tr("&Lightness first")); // TODO Use an icon instead of text
    
    // create the ColorPatch
    m_colorPatch = new ColorPatch();
    m_colorPatch->setColor(m_currentOpaqueColor.toRgbQColor());

    // Create widget for numerical value
    QWidget *tempNumericalWidget = initializeNumericPage();

    // Create layout for graphical and numerical selectors
    QHBoxLayout *tempSelectorLayout = new QHBoxLayout();
    tempSelectorLayout->addWidget(tempGraphicalTabWidget);
    tempSelectorLayout->addWidget(tempNumericalWidget);

    // Create alpha selector
    m_alphaSelector = new AlphaSelector(m_rgbColorSpace);
    QFormLayout *tempAlphaLayout = new QFormLayout();
    m_alphaSelectorLabel = new QLabel(tr("O&pacity:"));
    m_alphaSelector->registerAsBuddy(m_alphaSelectorLabel);
    tempAlphaLayout->addRow(m_alphaSelectorLabel, m_alphaSelector);
    m_alphaSelectorLabel->setVisible(false);
    m_alphaSelector->setVisible(false);

    // Create the default buttons
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &PerceptualColor::ColorDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &PerceptualColor::ColorDialog::reject);

    // Create the main layout
    QVBoxLayout *tempMainLayout = new QVBoxLayout();
    tempMainLayout->addLayout(tempSelectorLayout);
    tempMainLayout->addWidget(m_colorPatch);
    tempMainLayout->addLayout(tempAlphaLayout);
    tempMainLayout->addWidget(m_buttonBox);
    setLayout(tempMainLayout);
    
    // initialize signal-slot-connections
    connect(
        m_rgbRedSpinbox,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        &ColorDialog::readRgbNumericValues
    );
    connect(
        m_rgbGreenSpinbox,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        &ColorDialog::readRgbNumericValues
    );
    connect(
        m_rgbBlueSpinbox,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        &ColorDialog::readRgbNumericValues
    );
    connect(
        m_rgbLineEdit,
        &QLineEdit::editingFinished,
        this,
        &ColorDialog::readRgbHexValues
    );
    connect(
        m_hsvHueSpinbox,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        &ColorDialog::readHsvNumericValues
    );
    connect(
        m_hsvSaturationSpinbox,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        &ColorDialog::readHsvNumericValues
    );
    connect(
        m_hsvValueSpinbox,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        &ColorDialog::readHsvNumericValues
    );
    connect(
        qApp,
        &QApplication::focusChanged,
        this,
        &ColorDialog::handleFocusChange
    );
    connect(
        m_hlcLineEdit,
        &QLineEdit::returnPressed,
        this,
        &ColorDialog::readHlcNumericValues
    );
    connect(
        m_lchLightnessSelector,
        &GradientSelector::fractionChanged,
        this,
        &ColorDialog::readLightnessValue
    );
    connect(
        m_wheelColorPicker,
        &WheelColorPicker::currentColorChanged,
        this,
        &ColorDialog::setCurrentOpaqueColor
    );
    connect(
        m_chromaHueDiagram,
        &ChromaHueDiagram::colorChanged,
        this,
        &ColorDialog::setCurrentOpaqueColor
    );
}

void ColorDialog::handleFocusChange(QWidget *old, QWidget *now)
{
    if (old == m_hlcLineEdit) {
        // TODO Problem: This also reads the values even if they where not
        // changed. That's bad because it will make rounding errors.
        readHlcNumericValues();
    }
}

void ColorDialog::readHlcNumericValues()
{
    cmsCIELCh lch;
    QStringList temp = m_hlcLineEdit->text().split(QStringLiteral(u" "));
    if (temp.size() == 3) {
        lch.h = temp.at(0).toInt();
        lch.L = qMin(temp.at(1).toInt(), 100);
        lch.C = temp.at(2).toInt();
        setCurrentOpaqueColor(
            FullColorDescription(m_rgbColorSpace, lch, FullColorDescription::outOfGamutBehaviour::sacrifyChroma)
        );
    } else {
        m_hlcLineEdit->setText(
            QString(QStringLiteral(u"%1 %2 %3"))
                .arg(m_currentOpaqueColor.toLch().h, 0, 'f', 0)
                .arg(m_currentOpaqueColor.toLch().L, 0, 'f', 0)
                .arg(m_currentOpaqueColor.toLch().C, 0, 'f', 0)
        );
    }
}

// TODO Provide setWhatsThis help for widgets. For wheelcolorpicker and
// chromalightnessdiagram this could describe the keyboard controls and
// be integrated as default value in the class itself. For the other
// widgets, a setWhatsThis could be done here within WheelColorPicker,
// if appropriate.

QWidget* ColorDialog::initializeNumericPage()
{
    // Create HSV spin boxes
    const int hsvDecimals = 0;
    QHBoxLayout *tempHsvLayout = new QHBoxLayout;
    // TODO The current behaviour for pageStep = 10 is:
    // 356 -> 360 -> 0 -> 10
    // The expected behaviour would be:
    // 356 -> 6
    // This will likely require a new class inherited from QDoubleSpinBox.
    m_hsvHueSpinbox = new QDoubleSpinBox();
    m_hsvHueSpinbox->setAlignment(Qt::AlignRight);
    m_hsvHueSpinbox->setMaximum(360);
    m_hsvHueSpinbox->setWrapping(true);
    m_hsvHueSpinbox->setDecimals(hsvDecimals);
    m_hsvHueSpinbox->setWhatsThis(tr("<p>Hue</p><p>Range: 0–360</p>")); // Alternatively to – also ‥ could be used
    m_hsvSaturationSpinbox = new QDoubleSpinBox();
    m_hsvSaturationSpinbox->setAlignment(Qt::AlignRight);
    m_hsvSaturationSpinbox->setMaximum(255);
    m_hsvSaturationSpinbox->setDecimals(hsvDecimals);
    m_hsvSaturationSpinbox->setWhatsThis(tr("<p>Saturation</p><p>Range: 0–255</p>"));
    m_hsvValueSpinbox = new QDoubleSpinBox();
    m_hsvValueSpinbox->setAlignment(Qt::AlignRight);
    m_hsvValueSpinbox->setMaximum(255);
    m_hsvValueSpinbox->setDecimals(hsvDecimals);
    m_hsvValueSpinbox->setWhatsThis(tr("<p>Value</p><p>Range: 0–255</p>"));
    tempHsvLayout->addWidget(m_hsvHueSpinbox);
    tempHsvLayout->addWidget(m_hsvSaturationSpinbox);
    tempHsvLayout->addWidget(m_hsvValueSpinbox);

    // Create RGB spin boxes
    const int rgbDecimals = 0;
    QHBoxLayout *tempRgbLayout = new QHBoxLayout;
    m_rgbRedSpinbox = new QDoubleSpinBox();
    m_rgbRedSpinbox->setAlignment(Qt::AlignRight);
    m_rgbRedSpinbox->setMaximum(255);
    m_rgbRedSpinbox->setDecimals(rgbDecimals);
    m_rgbRedSpinbox->setWhatsThis(tr("<p>Red</p><p>Range: 0–255</p>"));
    m_rgbGreenSpinbox = new QDoubleSpinBox();
    m_rgbGreenSpinbox->setAlignment(Qt::AlignRight);
    m_rgbGreenSpinbox->setMaximum(255);
    m_rgbGreenSpinbox->setDecimals(rgbDecimals);
    m_rgbGreenSpinbox->setWhatsThis(tr("<p>Green</p><p>Range: 0–255</p>"));
    m_rgbBlueSpinbox = new QDoubleSpinBox();
    m_rgbBlueSpinbox->setAlignment(Qt::AlignRight);
    m_rgbBlueSpinbox->setMaximum(255);
    m_rgbBlueSpinbox->setDecimals(rgbDecimals);
    m_rgbBlueSpinbox->setWhatsThis(tr("<p>Blue</p><p>Range: 0–255</p>"));
    tempRgbLayout->addWidget(m_rgbRedSpinbox);
    tempRgbLayout->addWidget(m_rgbGreenSpinbox);
    tempRgbLayout->addWidget(m_rgbBlueSpinbox);

    // the hex style color representation
    m_rgbLineEdit = new QLineEdit();
    m_rgbLineEdit->setMaxLength(7);
    QRegularExpression tempRegularExpression(QStringLiteral(u"#?[0-9A-Fa-f]{0,6}"));
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(tempRegularExpression, this);
    m_rgbLineEdit->setValidator(validator);
    m_rgbLineEdit->setWhatsThis(
        tr(
            "<p>Hexadecimal color code, as used in HTML</p><p>#RRGGBB</p>"
            "<ul>RR: two-digit code for red</ul>"
            "<ul>GG: two-digit code for green</ul>"
            "<ul>BB: two-digit code for blue</ul>"
        )
    );

    // TODO allow entering "HLC 359 50 29" in form "h*<sub>ab</sub> 359 L* 50 C*<sub>ab</sub> 29"
    m_hlcLineEdit = new QLineEdit();
    QRegularExpression expression {QStringLiteral(u"\\d{1,3}\\s\\d{1,3}\\s\\d{1,3}")};
    m_hlcLineEdit->setValidator(new QRegularExpressionValidator(expression, this));

    // Create global layout, global widget, and return value
    QFormLayout *tempRgbPageFormLayout = new QFormLayout();
    QLabel *tempRgbLabel = new QLabel(tr("&RGB"));
    tempRgbLabel->setBuddy(m_rgbRedSpinbox);
    tempRgbPageFormLayout->addRow(tempRgbLabel, tempRgbLayout);
    tempRgbPageFormLayout->addRow(tr("He&x"), m_rgbLineEdit);
    QLabel *tempHsvLabel = new QLabel(tr("HS&V"));
    tempHsvLabel->setBuddy(m_hsvHueSpinbox);
    tempRgbPageFormLayout->addRow(tempHsvLabel, tempHsvLayout);
    tempRgbPageFormLayout->addRow(tr("HL&C"), m_hlcLineEdit);
    QWidget *tempWidget = new QWidget;
    tempWidget->setLayout(tempRgbPageFormLayout);
    tempWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    return tempWidget;
}

// No documentation here (documentation of properties and its getters are in the header)
QColorDialog::ColorDialogOptions ColorDialog::options() const
{
    return m_options;
}

/** Setter for options(). Note that
 * QColorDialog::ColorDialogOption::DontUseNativeDialog will always be set.
 * It cannot be disabled. */
void ColorDialog::setOption(QColorDialog::ColorDialogOption option, bool on)
{
    QColorDialog::ColorDialogOptions temp = m_options;
    temp.setFlag(option, on);
    setOptions(temp);
}

/** @brief Setter for options(). */
void ColorDialog::setOptions(QColorDialog::ColorDialogOptions options)
{
    // Save the new options
    m_options = options;
    // Correct QColorDialog::ColorDialogOption::DontUseNativeDialog which must be always on
    m_options.setFlag(QColorDialog::ColorDialogOption::DontUseNativeDialog, true);

    // Apply the new options
    m_alphaSelectorLabel->setVisible(
        m_options.testFlag(QColorDialog::ColorDialogOption::ShowAlphaChannel)
    );
    m_alphaSelector->setVisible(
        m_options.testFlag(QColorDialog::ColorDialogOption::ShowAlphaChannel)
    );
    m_buttonBox->setVisible(
        !m_options.testFlag(QColorDialog::ColorDialogOption::NoButtons)
    );
}

bool ColorDialog::testOption(QColorDialog::ColorDialogOption option) const
{
    return m_options.testFlag(option);
}

void ColorDialog::setVisible(bool visible)
{
    if (visible && (!isVisible())) {
        // Only delete the selected color if the new status is true
        // and the old one was false.
        m_selectedColor = QColor();
    }
    QDialog::setVisible(visible);
}

void ColorDialog::done(int result)
{
    if (result == QDialog::DialogCode::Accepted) {
        m_selectedColor = currentColor();
        Q_EMIT colorSelected(m_selectedColor);
    } else {
        m_selectedColor = QColor();
    }
    QDialog::done(result);
    if (m_receiverToBeDisconnected) {
        disconnect(
            this,
            SIGNAL(colorSelected(QColor)), // TODO would this work with &PerceptualColor::ColorDialog::colorSelected
            m_receiverToBeDisconnected,
            m_memberToBeDisconnected
        );
        m_receiverToBeDisconnected = 0;
    }
}

QColor ColorDialog::selectedColor() const
{
    return m_selectedColor;
}

QColor ColorDialog::getColor(
    const QColor &initial,
    QWidget *parent,
    const QString &title,
    QColorDialog::ColorDialogOptions options
)
{
    ColorDialog temp(parent);
    if (!title.isEmpty())
        temp.setWindowTitle(title);
    // Set options before setting current color. So we allow to
    // take the alpha value in account if the options specify
    // QColorDialog::ColorDialogOption::ShowAlphaChannel
    // actually. TODO Is this conform to QColorDialog behaviour?
    temp.setOptions(options);
    temp.setCurrentColor(initial); // Must be _after_ setOptions to allow alpha channel support
    temp.exec();
    return temp.selectedColor();
}

// TODO Use (on demand) two patches: old color (stays constant over the dialog call) and new color (as yet existing). Like in Scribus.

}
