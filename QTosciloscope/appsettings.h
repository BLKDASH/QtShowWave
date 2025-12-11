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

    // Setters
    void setEncoding(Encoding encoding);
    void setHexNewlineEnabled(bool enabled);
    void setKeywordHighlightEnabled(bool enabled);
    void setFontSize(int size);
    void setLastPortName(const QString &portName);

signals:
    void encodingChanged(AppSettings::Encoding encoding);
    void hexNewlineEnabledChanged(bool enabled);
    void keywordHighlightEnabledChanged(bool enabled);
    void fontSizeChanged(int size);

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
};

#endif // APPSETTINGS_H
