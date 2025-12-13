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
    
    // Serial port settings getters
    QString baudRate() const;
    int stopBitsIndex() const;
    int dataBitsIndex() const;
    int parityIndex() const;
    
    // Checkbox settings getters
    bool hexDisplayEnabled() const;
    bool timestampEnabled() const;
    bool clearAfterSendEnabled() const;
    bool hexSendEnabled() const;
    bool newLineEnabled() const;

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
    
    // Serial port settings setters
    void setBaudRate(const QString &baudRate);
    void setStopBitsIndex(int index);
    void setDataBitsIndex(int index);
    void setParityIndex(int index);
    
    // Checkbox settings setters
    void setHexDisplayEnabled(bool enabled);
    void setTimestampEnabled(bool enabled);
    void setClearAfterSendEnabled(bool enabled);
    void setHexSendEnabled(bool enabled);
    void setNewLineEnabled(bool enabled);

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
    QString m_fontFamily = "Default";
    QString m_lastPortName;
    bool m_darkModeEnabled = false;
    QSize m_windowSize;
    QByteArray m_splitterState;
    
    // Serial port settings
    QString m_baudRate = "115200";
    int m_stopBitsIndex = 0;
    int m_dataBitsIndex = 0;
    int m_parityIndex = 0;
    
    // Checkbox settings
    bool m_hexDisplayEnabled = false;
    bool m_timestampEnabled = false;
    bool m_clearAfterSendEnabled = false;
    bool m_hexSendEnabled = false;
    bool m_newLineEnabled = true;
};

#endif // APPSETTINGS_H
