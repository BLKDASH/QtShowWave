#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <atomic>

#include "serialconfig.h"

/**
 * @brief SerialWorker - 串口工作线程
 * 
 * 在独立线程中处理串口数据的接收和发送，避免阻塞主UI线程。
 * 使用 QThread 封装，串口在工作线程中运行。
 * 
 * Requirements: 1.1, 1.2, 1.4
 */
class SerialWorker : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit SerialWorker(QObject *parent = nullptr);

    /**
     * @brief 析构函数
     * 
     * 确保工作线程安全停止并释放资源。
     */
    ~SerialWorker();

    /**
     * @brief 检查串口是否正在运行
     * @return true 如果串口已打开且工作线程正在运行
     */
    bool isRunning() const;

    /**
     * @brief 获取当前工作线程ID（用于测试验证线程隔离）
     * @return 工作线程ID，如果未运行则返回 nullptr
     */
    Qt::HANDLE workerThreadId() const;

public slots:
    /**
     * @brief 启动串口通信
     * 
     * 在工作线程中打开串口并开始接收数据。
     * 如果配置无效或打开失败，将发出 errorOccurred 信号。
     * 
     * @param config 串口配置参数
     * Requirements: 1.1, 4.2, 4.3
     */
    void start(const SerialConfig &config);

    /**
     * @brief 停止串口通信
     * 
     * 关闭串口并停止工作线程。
     * 完成后发出 stopped 信号。
     */
    void stop();

    /**
     * @brief 发送数据到串口
     * 
     * 在工作线程中执行数据发送。
     * 如果串口未打开，将发出 errorOccurred 信号。
     * 
     * @param data 要发送的数据
     */
    void sendData(const QByteArray &data);

signals:
    /**
     * @brief 串口已成功启动
     * 
     * 当串口成功打开并开始接收数据时发出。
     */
    void started();

    /**
     * @brief 串口已停止
     * 
     * 当串口关闭并停止工作时发出。
     */
    void stopped();

    /**
     * @brief 接收到数据
     * 
     * 当从串口读取到数据时发出。
     * Requirements: 1.2
     * 
     * @param data 接收到的原始数据
     */
    void dataReceived(const QByteArray &data);

    /**
     * @brief 发生错误
     * 
     * 当发生任何错误时发出（配置无效、打开失败、读取错误等）。
     * Requirements: 1.4
     * 
     * @param error 错误描述
     */
    void errorOccurred(const QString &error);

private slots:
    /**
     * @brief 处理串口数据就绪
     * 
     * 当串口有数据可读时调用，读取所有可用数据并发出 dataReceived 信号。
     * Requirements: 1.2
     */
    void onReadyRead();

    /**
     * @brief 处理串口错误
     * 
     * 当串口发生错误时调用。
     * Requirements: 1.4
     * 
     * @param error 串口错误类型
     */
    void onSerialError(QSerialPort::SerialPortError error);

    /**
     * @brief 在工作线程中初始化串口
     * 
     * 由 start() 触发，在工作线程上下文中执行。
     */
    void doStart();

    /**
     * @brief 在工作线程中关闭串口
     * 
     * 由 stop() 触发，在工作线程上下文中执行。
     */
    void doStop();

    /**
     * @brief 在工作线程中发送数据
     * 
     * 由 sendData() 触发，在工作线程上下文中执行。
     * 
     * @param data 要发送的数据
     */
    void doSendData(const QByteArray &data);

private:
    /**
     * @brief 初始化工作线程
     */
    void initThread();

    /**
     * @brief 清理工作线程
     */
    void cleanupThread();

    QSerialPort *m_serial = nullptr;      ///< 串口对象（在工作线程中创建）
    QThread *m_thread = nullptr;          ///< 工作线程
    SerialConfig m_pendingConfig;         ///< 待应用的配置
    std::atomic<bool> m_running{false};   ///< 运行状态标志
    Qt::HANDLE m_workerThreadId = nullptr; ///< 工作线程ID
};

#endif // SERIALWORKER_H
