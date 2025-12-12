#include "appsettings.h"

AppSettings* AppSettings::s_instance = nullptr;

AppSettings::AppSettings(QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings("QTosciloscope", "Settings", this))
    , m_encoding(ANSI)
    , m_hexNewlineEnabled(true)
    , m_keywordHighlightEnabled(true)
    , m_fontSize(10)
    , m_darkModeEnabled(false)
{
    loadSettings();
}

AppSettings* AppSettings::instance()
{
    if (!s_instance) {
        s_instance = new AppSettings();
    }
    return s_instance;
}

void AppSettings::loadSettings()
{
    m_encoding = static_cast<Encoding>(m_settings->value("encoding", ANSI).toInt());
    m_hexNewlineEnabled = m_settings->value("hexNewlineEnabled", true).toBool();
    m_keywordHighlightEnabled = m_settings->value("keywordHighlightEnabled", true).toBool();
    m_fontSize = m_settings->value("fontSize", 10).toInt();
    m_fontFamily = m_settings->value("fontFamily", "HarmonyOS Sans SC").toString();
    m_lastPortName = m_settings->value("lastPortName", "").toString();
    m_darkModeEnabled = m_settings->value("darkModeEnabled", false).toBool();
    m_windowSize = m_settings->value("windowSize", QSize()).toSize();
    m_splitterState = m_settings->value("splitterState", QByteArray()).toByteArray();
    
    // Serial port settings
    m_baudRate = m_settings->value("baudRate", "115200").toString();
    m_stopBitsIndex = m_settings->value("stopBitsIndex", 0).toInt();
    m_dataBitsIndex = m_settings->value("dataBitsIndex", 0).toInt();
    m_parityIndex = m_settings->value("parityIndex", 0).toInt();
    
    // Checkbox settings
    m_hexDisplayEnabled = m_settings->value("hexDisplayEnabled", false).toBool();
    m_timestampEnabled = m_settings->value("timestampEnabled", false).toBool();
    m_clearAfterSendEnabled = m_settings->value("clearAfterSendEnabled", false).toBool();
    m_hexSendEnabled = m_settings->value("hexSendEnabled", false).toBool();
    m_newLineEnabled = m_settings->value("newLineEnabled", true).toBool();
}

void AppSettings::saveSettings()
{
    m_settings->setValue("encoding", static_cast<int>(m_encoding));
    m_settings->setValue("hexNewlineEnabled", m_hexNewlineEnabled);
    m_settings->setValue("keywordHighlightEnabled", m_keywordHighlightEnabled);
    m_settings->setValue("fontSize", m_fontSize);
    m_settings->setValue("fontFamily", m_fontFamily);
    m_settings->setValue("lastPortName", m_lastPortName);
    m_settings->setValue("darkModeEnabled", m_darkModeEnabled);
    m_settings->setValue("windowSize", m_windowSize);
    m_settings->setValue("splitterState", m_splitterState);
    
    // Serial port settings
    m_settings->setValue("baudRate", m_baudRate);
    m_settings->setValue("stopBitsIndex", m_stopBitsIndex);
    m_settings->setValue("dataBitsIndex", m_dataBitsIndex);
    m_settings->setValue("parityIndex", m_parityIndex);
    
    // Checkbox settings
    m_settings->setValue("hexDisplayEnabled", m_hexDisplayEnabled);
    m_settings->setValue("timestampEnabled", m_timestampEnabled);
    m_settings->setValue("clearAfterSendEnabled", m_clearAfterSendEnabled);
    m_settings->setValue("hexSendEnabled", m_hexSendEnabled);
    m_settings->setValue("newLineEnabled", m_newLineEnabled);
    
    m_settings->sync();
}

AppSettings::Encoding AppSettings::encoding() const
{
    return m_encoding;
}

bool AppSettings::hexNewlineEnabled() const
{
    return m_hexNewlineEnabled;
}

bool AppSettings::keywordHighlightEnabled() const
{
    return m_keywordHighlightEnabled;
}

int AppSettings::fontSize() const
{
    return m_fontSize;
}

QString AppSettings::lastPortName() const
{
    return m_lastPortName;
}

void AppSettings::setEncoding(Encoding encoding)
{
    if (m_encoding != encoding) {
        m_encoding = encoding;
        saveSettings();
        emit encodingChanged(m_encoding);
    }
}

void AppSettings::setHexNewlineEnabled(bool enabled)
{
    if (m_hexNewlineEnabled != enabled) {
        m_hexNewlineEnabled = enabled;
        saveSettings();
        emit hexNewlineEnabledChanged(m_hexNewlineEnabled);
    }
}

void AppSettings::setKeywordHighlightEnabled(bool enabled)
{
    if (m_keywordHighlightEnabled != enabled) {
        m_keywordHighlightEnabled = enabled;
        saveSettings();
        emit keywordHighlightEnabledChanged(m_keywordHighlightEnabled);
    }
}

void AppSettings::setFontSize(int size)
{
    // Clamp to valid range 6-24
    size = qBound(6, size, 24);
    if (m_fontSize != size) {
        m_fontSize = size;
        saveSettings();
        emit fontSizeChanged(m_fontSize);
    }
}

void AppSettings::setLastPortName(const QString &portName)
{
    if (m_lastPortName != portName) {
        m_lastPortName = portName;
        saveSettings();
    }
}

QString AppSettings::fontFamily() const
{
    return m_fontFamily;
}

void AppSettings::setFontFamily(const QString &family)
{
    if (m_fontFamily != family) {
        m_fontFamily = family;
        saveSettings();
        emit fontFamilyChanged(m_fontFamily);
    }
}

bool AppSettings::darkModeEnabled() const
{
    return m_darkModeEnabled;
}

void AppSettings::setDarkModeEnabled(bool enabled)
{
    if (m_darkModeEnabled != enabled) {
        m_darkModeEnabled = enabled;
        saveSettings();
        emit darkModeEnabledChanged(m_darkModeEnabled);
    }
}

QSize AppSettings::windowSize() const
{
    return m_windowSize;
}

void AppSettings::setWindowSize(const QSize &size)
{
    if (m_windowSize != size) {
        m_windowSize = size;
        saveSettings();
    }
}

QByteArray AppSettings::splitterState() const
{
    return m_splitterState;
}

void AppSettings::setSplitterState(const QByteArray &state)
{
    if (m_splitterState != state) {
        m_splitterState = state;
        saveSettings();
    }
}

// Serial port settings
QString AppSettings::baudRate() const { return m_baudRate; }
int AppSettings::stopBitsIndex() const { return m_stopBitsIndex; }
int AppSettings::dataBitsIndex() const { return m_dataBitsIndex; }
int AppSettings::parityIndex() const { return m_parityIndex; }

void AppSettings::setBaudRate(const QString &baudRate)
{
    if (m_baudRate != baudRate) {
        m_baudRate = baudRate;
        saveSettings();
    }
}

void AppSettings::setStopBitsIndex(int index)
{
    if (m_stopBitsIndex != index) {
        m_stopBitsIndex = index;
        saveSettings();
    }
}

void AppSettings::setDataBitsIndex(int index)
{
    if (m_dataBitsIndex != index) {
        m_dataBitsIndex = index;
        saveSettings();
    }
}

void AppSettings::setParityIndex(int index)
{
    if (m_parityIndex != index) {
        m_parityIndex = index;
        saveSettings();
    }
}

// Checkbox settings
bool AppSettings::hexDisplayEnabled() const { return m_hexDisplayEnabled; }
bool AppSettings::timestampEnabled() const { return m_timestampEnabled; }
bool AppSettings::clearAfterSendEnabled() const { return m_clearAfterSendEnabled; }
bool AppSettings::hexSendEnabled() const { return m_hexSendEnabled; }
bool AppSettings::newLineEnabled() const { return m_newLineEnabled; }

void AppSettings::setHexDisplayEnabled(bool enabled)
{
    if (m_hexDisplayEnabled != enabled) {
        m_hexDisplayEnabled = enabled;
        saveSettings();
    }
}

void AppSettings::setTimestampEnabled(bool enabled)
{
    if (m_timestampEnabled != enabled) {
        m_timestampEnabled = enabled;
        saveSettings();
    }
}

void AppSettings::setClearAfterSendEnabled(bool enabled)
{
    if (m_clearAfterSendEnabled != enabled) {
        m_clearAfterSendEnabled = enabled;
        saveSettings();
    }
}

void AppSettings::setHexSendEnabled(bool enabled)
{
    if (m_hexSendEnabled != enabled) {
        m_hexSendEnabled = enabled;
        saveSettings();
    }
}

void AppSettings::setNewLineEnabled(bool enabled)
{
    if (m_newLineEnabled != enabled) {
        m_newLineEnabled = enabled;
        saveSettings();
    }
}
