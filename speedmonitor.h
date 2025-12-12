#ifndef SPEEDMONITOR_H
#define SPEEDMONITOR_H

#include <QObject>
#include <QTimer>
#include <QString>

/**
 * @brief SpeedMonitor - 接收速度监控组件
 * 
 * 负责统计和计算数据接收速率，提供格式化显示功能。
 * 使用定时器每秒计算速度并发出更新信号。
 * 
 * Requirements: 1.1, 2.1, 3.1
 */
class SpeedMonitor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit SpeedMonitor(QObject *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~SpeedMonitor();

    /**
     * @brief 记录接收到的字节数
     * @param bytes 接收到的字节数（负数将被忽略）
     * Requirements: 3.1
     */
    void recordBytes(qint64 bytes);

    /**
     * @brief 重置所有计数器
     * Requirements: 3.2, 3.3
     */
    void reset();

    /**
     * @brief 启动速度监控
     * Requirements: 1.1
     */
    void start();

    /**
     * @brief 停止速度监控
     * Requirements: 1.3
     */
    void stop();

    /**
     * @brief 获取累计总字节数
     * @return 累计接收的总字节数
     */
    qint64 totalBytes() const;

    /**
     * @brief 获取当前速度
     * @return 当前速度（bytes/s）
     */
    double currentSpeed() const;

    /**
     * @brief 格式化速度显示
     * @param bytesPerSecond 速度值（bytes/s）
     * @return 格式化后的字符串（如 "512 bytes/s" 或 "1.5 KB/s"）
     * Requirements: 1.2
     */
    static QString formatSpeed(double bytesPerSecond);

    /**
     * @brief 格式化字节数显示
     * @param bytes 字节数
     * @return 格式化后的字符串（如 "512 B", "1.5 KB", "2.1 MB"）
     * Requirements: 3.4
     */
    static QString formatBytes(qint64 bytes);

signals:
    /**
     * @brief 速度更新信号（每秒发出）
     * @param bytesPerSecond 当前速度（bytes/s）
     * @param totalBytes 累计总字节数
     * Requirements: 2.1
     */
    void speedUpdated(double bytesPerSecond, qint64 totalBytes);

private slots:
    /**
     * @brief 定时器超时处理
     * 
     * 计算当前间隔内的速度并发出 speedUpdated 信号。
     */
    void onTimeout();

private:
    QTimer *m_timer;           ///< 定时器（1秒间隔）
    qint64 m_totalBytes;       ///< 累计总字节数
    qint64 m_intervalBytes;    ///< 当前间隔内的字节数
    double m_currentSpeed;     ///< 当前速度 (bytes/s)

    static constexpr int UPDATE_INTERVAL_MS = 1000;  ///< 更新间隔（毫秒）
};

#endif // SPEEDMONITOR_H
