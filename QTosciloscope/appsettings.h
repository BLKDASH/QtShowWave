#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>

class AppSettings : public QObject
{
    Q_OBJECT
public:
    enum Encoding { ANSI, UTF8, GBK };
    Q_ENUM(Encoding)

    static AppSettings* instance();

    // Getters
    Encoding encoding() const;
    bool hexNewlineEnabled() const;
    bool keywordHighlightEnabled() const;
    int fontSize() const;
    QString lastPortName() const;
    bool darkModeEnabled() const;

    // Setters
    void setEncoding(Encoding encoding);
    void setHexNewlineEnabled(bool enabled);
    void setKeywordHighlightEnabled(bool enabled);
    void setFontSize(int size);
    void setLastPortName(const QString &portName);
    void setDarkModeEnabled(bool enabled);

signals:
    void encodingChanged(AppSettings::Encoding encoding);
    void hexNewlineEnabledChanged(bool enabled);
    void keywordHighlightEnabledChanged(bool enabled);
    void fontSizeChanged(int size);
    void darkModeEnabledChanged(bool enabled);

private:
    explicit AppSettings(QObject *parent = nullptr);
    static AppSettings *s_instance;
    QSettings *m_settings;

    void loadSettings();
    void saveSettings();

    Encoding m_encoding = ANSI;
    bool m_hexNewlineEnabled = true;
    bool m_keywordHighlightEnabled = true;
    int m_fontSize = 9;
    QString m_lastPortName;
    bool m_darkModeEnabled = false;
};

#endif // APPSETTINGS_H
