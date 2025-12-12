#include "serialworker.h"

/**
 * @brief SerialWorker - 串口工作线程实现
 * 
 * Requirements: 1.1, 1.2, 1.4
 */

SerialWorker::SerialWorker(QObject *parent)
    : QObject(parent)
{
    initThread();
}

SerialWorker::~SerialWorker()
{
    // 确保停止并清理
    if (m_running) {
        // 同步停止，确保串口关闭
        QMetaObject::invokeMethod(this, "doStop", Qt::BlockingQueuedConnection);
    }
    
    // 确保 m_serial 被清理（防御性编程）
    if (m_serial) {
        delete m_serial;
        m_serial = nullptr;
    }
    
    cleanupThread();
}

bool SerialWorker::isRunning() const
{
    return m_running.load();
}

Qt::HANDLE SerialWorker::workerThreadId() const
{
    return m_workerThreadId;
}

void SerialWorker::initThread()
{
    m_thread = new QThread();
    
    // 将 SerialWorker 移动到工作线程
    this->moveToThread(m_thread);
    
    // 线程启动时记录线程ID
    connect(m_thread, &QThread::started, this, [this]() {
        m_workerThreadId = QThread::currentThreadId();
    });
    
    // 启动工作线程
    m_thread->start();
}

void SerialWorker::cleanupThread()
{
    if (m_thread) {
        m_thread->quit();
        m_thread->wait();
        delete m_thread;
        m_thread = nullptr;
    }
}

void SerialWorker::start(const SerialConfig &config)
{
    if (m_running) {
        emit errorOccurred(QStringLiteral("Serial port is already running"));
        return;
    }

    // 验证配置
    // Requirements: 4.2, 4.3
    if (!config.isValid()) {
        emit errorOccurred(config.validationError());
        return;
    }

    m_pendingConfig = config;
    
    // 在工作线程中执行实际的串口打开操作
    // Requirements: 1.1 - 串口在独立线程中运行
    QMetaObject::invokeMethod(this, "doStart", Qt::QueuedConnection);
}

void SerialWorker::stop()
{
    if (!m_running) {
        return;
    }

    // 在工作线程中执行实际的串口关闭操作
    QMetaObject::invokeMethod(this, "doStop", Qt::QueuedConnection);
}

void SerialWorker::sendData(const QByteArray &data)
{
    if (!m_running) {
        emit errorOccurred(QStringLiteral("Serial port is not open"));
        return;
    }

    if (data.isEmpty()) {
        return;
    }

    // 在工作线程中执行实际的数据发送
    QMetaObject::invokeMethod(this, "doSendData", Qt::QueuedConnection,
                              Q_ARG(QByteArray, data));
}

void SerialWorker::doStart()
{
    // 此方法在工作线程中执行
    // Requirements: 1.1 - 串口在独立线程中创建和运行
    
    m_serial = new QSerialPort();
    
    // 应用配置
    m_serial->setPortName(m_pendingConfig.portName);
    m_serial->setBaudRate(m_pendingConfig.baudRate);
    m_serial->setDataBits(m_pendingConfig.dataBits);
    m_serial->setStopBits(m_pendingConfig.stopBits);
    m_serial->setParity(m_pendingConfig.parity);
    m_serial->setFlowControl(m_pendingConfig.flowControl);
    m_serial->setReadBufferSize(m_pendingConfig.readBufferSize);

    // 连接信号
    connect(m_serial, &QSerialPort::readyRead, this, &SerialWorker::onReadyRead);
    connect(m_serial, &QSerialPort::errorOccurred, this, &SerialWorker::onSerialError);

    // 尝试打开串口
    // 注意：open() 失败时 Qt 会自动触发 errorOccurred 信号，
    // 所以这里不需要手动 emit，只需清理资源
    if (!m_serial->open(QIODevice::ReadWrite)) {
        delete m_serial;
        m_serial = nullptr;
        return;
    }

    m_running = true;
    emit started();
}

void SerialWorker::doStop()
{
    // 此方法在工作线程中执行
    
    if (m_serial) {
        m_serial->close();
        delete m_serial;
        m_serial = nullptr;
    }

    m_running = false;
    emit stopped();
}

void SerialWorker::doSendData(const QByteArray &data)
{
    // 此方法在工作线程中执行
    
    if (!m_serial || !m_serial->isOpen()) {
        emit errorOccurred(QStringLiteral("Serial port is not open"));
        return;
    }

    qint64 bytesWritten = m_serial->write(data);
    if (bytesWritten == -1) {
        emit errorOccurred(QStringLiteral("Failed to write data: %1").arg(m_serial->errorString()));
    } else if (bytesWritten != data.size()) {
        emit errorOccurred(QStringLiteral("Incomplete write: %1 of %2 bytes written")
                          .arg(bytesWritten).arg(data.size()));
    }
}

void SerialWorker::onReadyRead()
{
    // 此方法在工作线程中执行
    // Requirements: 1.2 - 在工作线程中读取数据，不阻塞UI线程
    
    if (!m_serial) {
        return;
    }

    QByteArray data = m_serial->readAll();
    if (!data.isEmpty()) {
        emit dataReceived(data);
    }
}

void SerialWorker::onSerialError(QSerialPort::SerialPortError error)
{
    // 此方法在工作线程中执行
    // Requirements: 1.4 - 发生错误时发出信号通知主线程
    
    if (error == QSerialPort::NoError) {
        return;
    }

    QString errorString;
    switch (error) {
    case QSerialPort::DeviceNotFoundError:
        errorString = QStringLiteral("Device not found");
        break;
    case QSerialPort::PermissionError:
        errorString = QStringLiteral("Permission denied");
        break;
    case QSerialPort::OpenError:
        errorString = QStringLiteral("Failed to open port");
        break;
    case QSerialPort::WriteError:
        errorString = QStringLiteral("Write error");
        break;
    case QSerialPort::ReadError:
        errorString = QStringLiteral("Read error");
        break;
    case QSerialPort::ResourceError:
        errorString = QStringLiteral("Resource error (device may have been disconnected)");
        break;
    case QSerialPort::TimeoutError:
        errorString = QStringLiteral("Timeout error");
        break;
    default:
        errorString = m_serial ? m_serial->errorString() : QStringLiteral("Unknown error");
        break;
    }

    emit errorOccurred(errorString);
}
