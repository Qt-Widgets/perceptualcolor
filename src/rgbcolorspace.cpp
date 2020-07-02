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
#include "PerceptualColor/rgbcolorspace.h"

#include "PerceptualColor/helper.h"

#include <QDebug>

namespace PerceptualColor {

/** @brief Default constructor
 * 
 * Creates an sRGB color space.
 */
RgbColorSpace::RgbColorSpace(QObject *parent) : QObject(parent)
{
    // Create an ICC v4 profile object for the Lab color space.
    // NULL means: Default white point (D50) // TODO Does this make sense? sRGB white point is D65!
    cmsHPROFILE labProfileHandle = cmsCreateLab4Profile(NULL);
    // Create an ICC profile object for the sRGB color space.
    cmsHPROFILE rgbProfileHandle = cmsCreate_sRGBProfile();
    m_description = getInformationFromProfile(rgbProfileHandle, cmsInfoDescription);
    m_description = tr("sRGB"); // TODO Do this only if the build-in sRGB is used, not when an actual external ICC profile is used.
    // Create the transforms
    m_transformLabToRgbHandle = cmsCreateTransform(
        labProfileHandle,             // input profile handle
        TYPE_Lab_DBL,                 // input buffer format
        rgbProfileHandle,             // output profile handle
        TYPE_RGB_DBL,                 // output buffer format
        INTENT_ABSOLUTE_COLORIMETRIC, // rendering intent
        0                             // flags
    );
    m_transformLabToRgb16Handle = cmsCreateTransform(
        labProfileHandle,             // input profile handle
        TYPE_Lab_DBL,                 // input buffer format
        rgbProfileHandle,             // output profile handle
        TYPE_RGB_16,                  // output buffer format
        INTENT_ABSOLUTE_COLORIMETRIC, // rendering intent
        0                             // flags
    );
    m_transformRgbToLabHandle = cmsCreateTransform(
        rgbProfileHandle,             // input profile handle
        TYPE_RGB_DBL,                 // input buffer format
        labProfileHandle,             // output profile handle
        TYPE_Lab_DBL,                 // output buffer format
        INTENT_ABSOLUTE_COLORIMETRIC, // rendering intent
        0                             // flags
    );
    // Close profile (free memory)
    cmsCloseProfile(labProfileHandle);
    cmsCloseProfile(rgbProfileHandle);

    // Now we know for sure that lowerChroma is in-gamut and upperChroma is out-of-gamut…
    cmsCIELCh candidate;
    candidate.L = 0;
    candidate.C = 0;
    candidate.h = 0;
    while (!inGamut(candidate) && (candidate.L < 100)) {
        candidate.L += Helper::gamutPrecision;
    }
    m_blackpointL = candidate.L;
    candidate.L = 100;
    while (!inGamut(candidate) && (candidate.L > 0)) {
        candidate.L -= Helper::gamutPrecision;
    }
    m_whitepointL = candidate.L;
    if (m_whitepointL <= m_blackpointL) {
        qCritical() << "Unable to find blackpoint and whitepoint on gray axis.";
        throw 0;
    }
}

/** @brief Destructor */
RgbColorSpace::~RgbColorSpace()
{
    cmsDeleteTransform(m_transformLabToRgb16Handle);
    cmsDeleteTransform(m_transformLabToRgbHandle);
    cmsDeleteTransform(m_transformRgbToLabHandle);
}

/** @brief The darkest in-gamut point on the L* axis.
 * 
 * @sa whitepointL */
qreal RgbColorSpace::blackpointL() const
{
    return m_blackpointL;
}

/** @brief The lightest in-gamut point on the L* axis.
 * 
 * @sa blackpointL() */
qreal RgbColorSpace::whitepointL() const
{
    return m_whitepointL;
}

/** @brief Calculates the Lab value
 * 
 * @param rgbColor the color that will be converted. (If this is not an RGB color, it
 * will be converted first into an RGB color by QColor methods.)
 * @returns If the color is valid, the corresponding LCh value might also be invalid.
 */
cmsCIELab RgbColorSpace::colorLab(const QColor &rgbColor) const
{
    Helper::cmsRGB my_rgb;
    my_rgb.red = rgbColor.redF();
    my_rgb.green = rgbColor.greenF();
    my_rgb.blue = rgbColor.blueF();
    return colorLab(my_rgb);
}

/** @brief Calculates the Lab value
 * 
 * @param rgbColor the color that will be converted. (If this is not an RGB color, it
 * will be converted first into an RGB color by QColor methods.)
 * @returns If the color is valid, the corresponding LCh value might also be invalid.
 */
cmsCIELab RgbColorSpace::colorLab(const Helper::cmsRGB &rgb) const
{
    cmsCIELab lab;
    cmsDoTransform(m_transformRgbToLabHandle, &rgb, &lab, 1); // convert exactly 1 value
    return lab;
}

/** @brief Calculates the RGB value
 * 
 * @param Lab a L*a*b* color
 * @returns If the color is within the RGB gammut, a QColor with the RGB values.
 * An invald QColor otherwise.
 */
QColor RgbColorSpace::colorRgb(const cmsCIELab &Lab) const
{
    QColor temp; // By default, without initialization this is an invalid color
    Helper::cmsRGB rgb;
    cmsDoTransform(m_transformLabToRgbHandle, &Lab, &rgb, 1); // convert exactly 1 value
    if (Helper::inRange<cmsFloat64Number>(0, rgb.red, 1) &&
        Helper::inRange<cmsFloat64Number>(0, rgb.green, 1) &&
        Helper::inRange<cmsFloat64Number>(0, rgb.blue, 1)) {
        // We are within the gamut
        temp = QColor::fromRgbF(rgb.red, rgb.green, rgb.blue);
    }
    return temp;
}

/** @brief Calculates the RGB value
 * 
 * @param LCh an LCh color
 * @returns If the color is within the RGB gammut, a QColor with the RGB values.
 * An invald QColor otherwise.
 */
QColor RgbColorSpace::colorRgb(const cmsCIELCh &LCh) const
{
    cmsCIELab Lab; // uses cmsFloat64Number internally
    // convert from LCh to Lab
    cmsLCh2Lab(&Lab, &LCh); 
    return colorRgb(Lab);
}

Helper::cmsRGB RgbColorSpace::colorRgbBoundSimple(const cmsCIELab &Lab) const
{
    cmsUInt16Number rgb_int[3];
    cmsDoTransform(m_transformLabToRgb16Handle, &Lab, rgb_int, 1); // convert exactly 1 value
    Helper::cmsRGB temp;
    temp.red = rgb_int[0] / static_cast<qreal>(65535);
    temp.green = rgb_int[1] / static_cast<qreal>(65535);
    temp.blue = rgb_int[2] / static_cast<qreal>(65535);
    return temp;
}

/** @brief Calculates the RGB value
 * 
 * @param Lab a L*a*b* color
 * @returns If the color is within the RGB gammut, a QColor with the RGB values.
 * A nearby (in-gamut) RGB QColor otherwise.
 */
QColor RgbColorSpace::colorRgbBound(const cmsCIELab &Lab) const
{
    Helper::cmsRGB temp = colorRgbBoundSimple(Lab);
    return QColor::fromRgbF(temp.red, temp.green, temp.blue);
}

/** @brief Calculates the RGB value
 * 
 * @param LCh an LCh color
 * @returns If the color is within the RGB gammut, a QColor with the RGB values.
 * A nearby (in-gamut) RGB QColor otherwise.
 */
QColor RgbColorSpace::colorRgbBound(const cmsCIELCh &LCh) const
{
    cmsCIELab Lab; // uses cmsFloat64Number internally
    // convert from LCh to Lab
    cmsLCh2Lab(&Lab, &LCh); 
    return colorRgbBound(Lab);
}

// TODO What to do with in-gamut tests if little cms has fallen back to bounded mode because of too complicate profiles? Out in-gamut detection would not work anymore!

/** @brief check if an LCh value is within a specific RGB gamut
 * @param lightness The lightness value
 * @param chroma The chroma value
 * @param hue The hue value (angle in degree)
 * @returns Returns true if lightness/chroma/hue is in the specified RGB gamut. Returns false otherwise.
 */
bool RgbColorSpace::inGamut(
    const cmsFloat64Number lightness,
    const cmsFloat64Number chroma,
    const cmsFloat64Number hue
)
{
    // variables
    cmsCIELCh LCh; // uses cmsFloat64Number internally

    // code
    LCh.L = lightness;
    LCh.C = chroma;
    LCh.h = hue;
    return inGamut(LCh);
}

/** @brief check if an LCh value is within a specific RGB gamut
 * @param LCh the LCh color
 * @returns Returns true if lightness/chroma/hue is in the specified RGB gamut. Returns false otherwise.
 */
bool RgbColorSpace::inGamut(const cmsCIELCh &LCh)
{
    // variables
    cmsCIELab Lab; // uses cmsFloat64Number internally
    Helper::cmsRGB rgb;

    // code
    cmsLCh2Lab(&Lab, &LCh); // TODO no normalization necessary previously?
    cmsDoTransform(m_transformLabToRgbHandle, &Lab, &rgb, 1); // convert exactly 1 value

    return (
        Helper::inRange<cmsFloat64Number>(0, rgb.red, 1) &&
        Helper::inRange<cmsFloat64Number>(0, rgb.green, 1) &&
        Helper::inRange<cmsFloat64Number>(0, rgb.blue, 1)
    );
}

/** Returns the description of the RGB color space. */
QString RgbColorSpace::description() const
{
    return m_description;
}

/** @brief Get information from the ICC profile
 * 
 * Fetches information from the ICC profile.
 * @param profileHandle the profile in which is searched
 * @param infoType the type of information that is searched
 * @returns a QString with the information. It prefers the information in the
 * current locale (country code and language code as used by QLocale()). If
 * this is not available in the ICC profile, it silently falls back to an
 * available localization. The returned QString() might be empty if the
 * information is not available in the ICC profile.
 */
QString RgbColorSpace::getInformationFromProfile(cmsHPROFILE profileHandle, cmsInfoType infoType)
{
    char languageCode[3] = "en"; // recommended default value following LittleCMS documentation
    char countryCode[3] = "US";  // recommended default value following LittleCMS documentation
    // Update languageCode and countryCode to actual values if possible
    // TODO What is the result does not contain, as expected, 2 x 2 characters? Crash!
    QStringList list = QLocale().name().split('_');
    if (list.at(0).size() == 2) {
        languageCode[0] = list.at(0).at(0).toLatin1();
        languageCode[1] = list.at(0).at(1).toLatin1();
    }
    if (list.at(1).size() == 2) {
        countryCode[0] = list.at(1).at(0).toLatin1();
        countryCode[1] = list.at(1).at(1).toLatin1();
    }

    // Get the size of the buffer that cmsGetProfileInfo needs to return a value
    cmsUInt32Number resultLength = cmsGetProfileInfo(
        profileHandle,  // profile in which we search
        infoType,       // the type of information we search
        languageCode,   // the preferred language in which we want to get the information
        countryCode,    // the preferred country in which we want to get the information
        NULL,           // do not actually return the information, just calculate the required buffer size
        0               // do not actually return the information, just calculate the required buffer size
    );
    // For the actual buffer, we add +1 which will later serve to guarantee zero-terminated string
    cmsUInt32Number bufferLength = resultLength + 1;
    // Allocate the buffer
    wchar_t *buffer = new wchar_t [bufferLength];
    // Initialize the buffer with NULL
    for (int i = 0; i < bufferLength - 1; ++i) {
        *(buffer + i) = 0;
    }
    // Get the actual description
    cmsGetProfileInfo(
        profileHandle,  // profile in which we search
        infoType,       // the type of information we search
        languageCode,   // the preferred language in which we want to get the information
        countryCode,    // the preferred country in which we want to get the information
        buffer,         // the buffer into which the requested information will be writen
        resultLength    // the buffer size as previously calculated
    );
    /* QString::fromWCharArray allows to specify the number of elements to read.
     * But cmsGetProfileInfo returns often strings that are smaller than the
     * previewed buffer size. So it's better to read-in the buffer as
     * null-terminated buffer.
     */
    // Make absolutely sure the buffer is null-terminated by marking its last
    // element (the one that was the +1 "extra" element) as NULL.
    *(buffer + (bufferLength - 1)) = 0;
    // Read-in from the buffer
    QString result = QString::fromWCharArray(
        buffer, // read from the buffer
        -1      // read until the first NULL element
    );
    // Free allocated memory of the buffer
    delete [] buffer;
    // Return
    return result;
}

}
