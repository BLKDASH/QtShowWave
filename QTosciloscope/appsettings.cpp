#include "appsettings.h"

AppSettings* AppSettings::s_instance = nullptr;

AppSettings::AppSettings(QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings("QTosciloscope", "Settings", this))
    , m_encoding(ANSI)
    , m_hexNewlineEnabled(true)
    , m_keywordHighlightEnabled(true)
    , m_fontSize(9)
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
    m_fontSize = m_settings->value("fontSize", 9).toInt();
    m_lastPortName = m_settings->value("lastPortName", "").toString();
}

void AppSettings::saveSettings()
{
    m_settings->setValue("encoding", static_cast<int>(m_encoding));
    m_settings->setValue("hexNewlineEnabled", m_hexNewlineEnabled);
    m_settings->setValue("keywordHighlightEnabled", m_keywordHighlightEnabled);
    m_settings->setValue("fontSize", m_fontSize);
    m_settings->setValue("lastPortName", m_lastPortName);
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
