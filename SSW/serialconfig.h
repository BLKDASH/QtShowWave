#ifndef SERIALCONFIG_H
#define SERIALCONFIG_H

#include <QString>
#include <QSerialPort>

/**
 * @brief SerialConfig - 串口配置数据结构
 * 
 * 封装所有串口参数，提供配置验证功能。
 * Requirements: 4.1, 4.2, 4.3
 */
struct SerialConfig {
    QString portName;
    qint32 baudRate = 115200;
    QSerialPort::DataBits dataBits = QSerialPort::Data8;
    QSerialPort::StopBits stopBits = QSerialPort::OneStop;
    QSerialPort::Parity parity = QSerialPort::NoParity;
    QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
    int readBufferSize = 4096;

    /**
     * @brief 验证配置参数是否有效
     * @return true 如果所有参数有效，否则返回 false
     */
    bool isValid() const {
        // 验证端口名不为空
        if (portName.isEmpty()) {
            return false;
        }

        // 验证波特率为正数且在合理范围内
        if (baudRate <= 0) {
            return false;
        }

        // 验证波特率是否为常用值
        static const QList<qint32> validBaudRates = {
            1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 
            230400, 460800, 921600
        };
        if (!validBaudRates.contains(baudRate)) {
            return false;
        }

        // 验证读取缓冲区大小
        if (readBufferSize <= 0) {
            return false;
        }

        return true;
    }

    /**
     * @brief 获取配置验证错误信息
     * @return 错误描述字符串，如果配置有效则返回空字符串
     */
    QString validationError() const {
        if (portName.isEmpty()) {
            return QStringLiteral("Port name cannot be empty");
        }

        if (baudRate <= 0) {
            return QStringLiteral("Baud rate must be a positive number");
        }

        static const QList<qint32> validBaudRates = {
            1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200,
            230400, 460800, 921600
        };
        if (!validBaudRates.contains(baudRate)) {
            return QStringLiteral("Invalid baud rate: %1. Supported rates: 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600").arg(baudRate);
        }

        if (readBufferSize <= 0) {
            return QStringLiteral("Read buffer size must be a positive number");
        }

        return QString();
    }
};

#endif // SERIALCONFIG_H
