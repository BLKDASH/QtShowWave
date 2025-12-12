#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QScrollBar>

#include "serialworker.h"
#include "dataprocessor.h"
#include "databuffer.h"
#include "serialconfig.h"
#include "keywordhighlighter.h"
#include "speedmonitor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

/**
 * @brief Widget - 主界面控件
 * 
 * 负责UI显示和用户交互，集成 SerialWorker、DataProcessor、DataBuffer 组件。
 * 实现定时批量刷新机制和自动滚动暂停/恢复逻辑。
 * 
 * Requirements: 1.3, 6.1, 6.2, 6.3, 6.4
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    static constexpr int REFRESH_INTERVAL_MS = 33; // ~30 FPS

private slots:
    void on_clear_clicked();
    void on_cbPortName_clicked();
    void on_open_clicked();
    void on_send_clicked();
    void on_clearSend_clicked();

    void onRefreshTimeout();
    void onDataReceived(const QByteArray &data);
    void onDataProcessed(const QString &text);
    void onSerialError(const QString &error);
    void onSerialStarted();
    void onSerialStopped();
    void onScrollValueChanged(int value);
    void onSpeedUpdated(double bytesPerSecond, qint64 totalBytes);
    void onSplitterMoved(int pos, int index);

    void on_openSetButton_clicked();

private:
    void setupConnections();
    void updatePortList();
    void setPortControlsEnabled(bool enabled);
    void appendToDisplay(const QString &text);
    void showSystemMessage(const QString &message);
    void performSend();
    SerialConfig buildConfig() const;
    void applyDarkMode(bool enabled);

    Ui::Widget *ui;
    SerialWorker *m_worker;
    DataProcessor *m_processor;
    DataBuffer *m_buffer;
    QTimer *m_refreshTimer;
    KeywordHighlighter *m_highlighter;
    SpeedMonitor *m_speedMonitor;
    QString m_pendingText;
    bool m_autoScroll = true;
};

#endif // WIDGET_H
