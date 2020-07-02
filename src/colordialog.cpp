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

#include "PerceptualColor/alphaselector.h"
#include "PerceptualColor/chromalightnessdiagram.h"
#include "PerceptualColor/colorpatch.h"
#include "PerceptualColor/helper.h"
#include "PerceptualColor/simplecolorwheel.h"
#include "PerceptualColor/wheelcolorpicker.h"

#include <QApplication>
#include <QColorDialog>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QSlider>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>

namespace PerceptualColor {

/** @brief Constructor
 * 
 * @param parent pointer to the parent widget
 */
ColorDialog::ColorDialog(QWidget *parent) : QDialog(parent)
{
    initialize();
    setCurrentColor(QColor(Qt::white)); // That's what QColorDialog does
}

/** @brief Constructor providing initial color
 * 
 * @param initial the initially chosen color of the
 * dialog (restrictions of setColor() apply here also)
 * @param parent pointer to the parent widget
 */
ColorDialog::ColorDialog(const QColor &initial, QWidget *parent) : QDialog(parent)
{
    initialize();
    setCurrentColor(initial);
}

ColorDialog::~ColorDialog()
{
    delete m_rgbColorSpace;
    // TODO how to delete the sub-layouts that do not have parents? Are they deleted automatically by the layout in which they are embedded?
}

/** @brief Currently selected color.
 * 
 * If alpha handling is enabled in options(), than the corresponding
 * alpha value is included. Otherwise a completly opaque color is
 * returned. */
QColor ColorDialog::currentColor() const
{
    QColor temp;
    temp = m_currentOpaqueFullColor.toRgbQColor();
    temp.setAlphaF(m_alphaSelector->alpha());
    return temp;
}

/** @brief Setter for currentColor() property.
 * 
 * @param color the new color
 * \post The property currentColor() is adapted as follows:
 * - The RGB part of currentColor() will be the RGB part of <tt>color</tt>.
 * - The alpha channel of currentColor() will be the alpha channel of
 *   <tt>color</tt> if at the moment the
 *   <tt>QColorDialog::ColorDialogOption::ShowAlphaChannel</tt> option is set.
 *   It will be fully opaque otherwise.
 */
void ColorDialog::setCurrentColor(const QColor& color)
{
    QColor temp;
    if (color.isValid()) {
        temp = color;
    } else {
        // For invalid colors same behaviour as QColorDialog
        temp = QColor(Qt::black);
    }
    if (testOption(QColorDialog::ColorDialogOption::ShowAlphaChannel)) {
        m_alphaSelector->setAlpha(temp.alphaF());
    } else {
        m_alphaSelector->setAlpha(1);
    }
    setCurrentOpaqueFullColor(FullColorDescription(m_rgbColorSpace, temp));
}

void ColorDialog::open(QObject *receiver, const char *member)
{
    connect(this, SIGNAL(colorSelected(QColor)), receiver, member);
    m_receiverToBeDisconnected = receiver;
    m_memberToBeDisconnected = member;
    QDialog::open();
}

/** @param color the color. It's alpha channel is ignored. */
void ColorDialog::setCurrentOpaqueQColor(const QColor& color)
{
    setCurrentOpaqueFullColor(FullColorDescription(m_rgbColorSpace, color));
}

// TODO Everywhere in this library incoming QColor valued should be converter to RGB!

/** Updates m_currentPlainFullColor and the widgets. @warning This function ignores the alpha component! */
void ColorDialog::setCurrentOpaqueFullColor(const FullColorDescription& color)
{
    if (m_isColorChangeInProgress || (!color.isValid()) || (color == m_currentOpaqueFullColor)) {
        return;
    }
    m_isColorChangeInProgress = true;
    QColor oldQColor = currentColor(); // currentColor() provides correct alpha treatment
    m_currentOpaqueFullColor = color;
    updateOpaqueColorWidgets(m_currentOpaqueFullColor);
    if (currentColor() != oldQColor) {
        Q_EMIT currentColorChanged(currentColor());
    }
    m_isColorChangeInProgress = false;
}

/** @brief Update the plain color widgets.
 * 
 * Does not update the alpha-channel related widgets.
 */
void ColorDialog::updateOpaqueColorWidgets(const FullColorDescription &color)
{
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
    m_wheelColorPicker->setCurrentColor(m_currentOpaqueFullColor);
    m_alphaSelector->setColor(m_currentOpaqueFullColor);
}

void ColorDialog::readLightnessValue()
{
    cmsCIELCh lch = m_currentOpaqueFullColor.toLch();
    lch.L = m_lchLightnessSelector->fraction() * 100;
    setCurrentOpaqueFullColor(
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
        setCurrentOpaqueQColor(temp);
    }
    // Return to the finally considered value.
    m_rgbLineEdit->setText(m_currentOpaqueFullColor.toRgbQColor().name());
}

/** Basic initialization. Code that is shared between the various
 * overloaded constructors.
 */
void ColorDialog::initialize()
{
    // initialize color space
    m_rgbColorSpace = new RgbColorSpace();

    // initialize the options
    m_options = QColorDialog::ColorDialogOption::DontUseNativeDialog;

    // create the graphical selectors
    m_wheelColorPicker = new WheelColorPicker(m_rgbColorSpace);
    m_currentOpaqueFullColor = m_wheelColorPicker->currentColor();
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
    m_colorPatch->setColor(m_currentOpaqueFullColor.toRgbQColor());

    // Create widget for numerical value
    QWidget *tempNumericalWidget = initializeRgbPage();
    initializeLabPage(); // just to create the widgets, but do not actually use it in the layout.

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
        &ColorDialog::setCurrentOpaqueFullColor
    );
    connect(
        m_chromaHueDiagram,
        &ChromaHueDiagram::colorChanged,
        this,
        &ColorDialog::setCurrentOpaqueFullColor
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
        setCurrentOpaqueFullColor(
            FullColorDescription(m_rgbColorSpace, lch, FullColorDescription::outOfGamutBehaviour::sacrifyChroma)
        );
    } else {
        updateOpaqueColorWidgets(m_currentOpaqueFullColor);
    }
}

QWidget* ColorDialog::initializeLabPage()
{
    // TODO this is dead code currently. Remove it?

    // Create HLC group box
    QGroupBox *tempLchGrouBox = new QGroupBox();
    tempLchGrouBox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum); // TODO that's bad choise
    QFormLayout *tempLchFormLayout = new QFormLayout();
    tempLchGrouBox->setLayout(tempLchFormLayout);
    // TODO allow entering "HLC 359 50 29" in form "h*<sub>ab</sub> 359 L* 50 C*<sub>ab</sub> 29"
    m_hlcLineEdit = new QLineEdit();
    QRegularExpression expression {QStringLiteral(u"\\d{1,3}\\s\\d{1,3}\\s\\d{1,3}")};
    m_hlcLineEdit->setValidator(new QRegularExpressionValidator(expression, this));
    tempLchFormLayout->addRow(tr("&HLC"), m_hlcLineEdit);
    
    return tempLchGrouBox;
}

// TODO Provide setWhatsThis help for widgets. For wheelcolorpicker and
// chromalightnessdiagram this could describe the keyboard controls and
// be integrated as default value in the class itself. For the other
// widgets, a setWhatsThis could be done here within WheelColorPicker,
// if appropriate.

QWidget* ColorDialog::initializeRgbPage()
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

    // Create global layout, global widget, and return value
    QFormLayout *tempRgbPageFormLayout = new QFormLayout();
    QLabel *tempRgbLabel = new QLabel(tr("&RGB"));
    tempRgbLabel->setBuddy(m_rgbRedSpinbox);
    tempRgbPageFormLayout->addRow(tempRgbLabel, tempRgbLayout);
    tempRgbPageFormLayout->addRow(tr("He&x"), m_rgbLineEdit);
    QLabel *tempHsvLabel = new QLabel(tr("HS&V"));
    tempHsvLabel->setBuddy(m_hsvHueSpinbox);
    tempRgbPageFormLayout->addRow(tempHsvLabel, tempHsvLayout);
    QWidget *tempWidget = new QWidget;
    tempWidget->setLayout(tempRgbPageFormLayout);
    tempWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    return tempWidget;
}

/** @brief Various configuration options
 * 
 * Stores some configuration options for this dialog. Note that
 * QColorDialog::ColorDialogOption::DontUseNativeDialog will always be set.
 * It cannot be disabled.
 */
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

/** Setter for options(). Note that
 * QColorDialog::ColorDialogOption::DontUseNativeDialog will always be set.
 * It cannot be disabled. */
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
            SIGNAL(colorSelected(QColor)),
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
    QColorDialog temp(parent);
    if (!title.isEmpty())
        temp.setWindowTitle(title);
    temp.setCurrentColor(initial);
    temp.setOptions(options);
    temp.exec();
    return temp.selectedColor();
}

// TODO Use (on demand) two patches: old color (stays constant over the dialog call) and new color (as yet existing). Like in Scribus.

}
