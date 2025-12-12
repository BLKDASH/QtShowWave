#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QSize>

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
    QString fontFamily() const;
    QString lastPortName() const;
    bool darkModeEnabled() const;
    QSize windowSize() const;
    QByteArray splitterState() const;

    // Setters
    void setEncoding(Encoding encoding);
    void setHexNewlineEnabled(bool enabled);
    void setKeywordHighlightEnabled(bool enabled);
    void setFontSize(int size);
    void setFontFamily(const QString &family);
    void setLastPortName(const QString &portName);
    void setDarkModeEnabled(bool enabled);
    void setWindowSize(const QSize &size);
    void setSplitterState(const QByteArray &state);

signals:
    void encodingChanged(AppSettings::Encoding encoding);
    void hexNewlineEnabledChanged(bool enabled);
    void keywordHighlightEnabledChanged(bool enabled);
    void fontSizeChanged(int size);
    void fontFamilyChanged(const QString &family);
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
    int m_fontSize = 10;
    QString m_fontFamily = "HarmonyOS Sans SC";
    QString m_lastPortName;
    bool m_darkModeEnabled = false;
    QSize m_windowSize;
    QByteArray m_splitterState;
};

#endif // APPSETTINGS_H
