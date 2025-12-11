#include "dataprocessor.h"
#include <QStringDecoder>

/**
 * @brief DataProcessor 实现
 * 
 * Requirements: 3.1, 3.2, 3.3
 */

DataProcessor::DataProcessor(QObject *parent)
    : QObject(parent)
    , m_format(ASCII)
    , m_timestampEnabled(false)
    , m_encoding(AppSettings::ANSI)
    , m_hexNewlineEnabled(true)
{
}

void DataProcessor::setFormat(Format format)
{
    m_format = format;
}

DataProcessor::Format DataProcessor::format() const
{
    return m_format;
}

void DataProcessor::setTimestampEnabled(bool enabled)
{
    m_timestampEnabled = enabled;
}

bool DataProcessor::isTimestampEnabled() const
{
    return m_timestampEnabled;
}

void DataProcessor::setEncoding(AppSettings::Encoding encoding)
{
    m_encoding = encoding;
}

AppSettings::Encoding DataProcessor::encoding() const
{
    return m_encoding;
}

void DataProcessor::setHexNewlineEnabled(bool enabled)
{
    m_hexNewlineEnabled = enabled;
}

bool DataProcessor::isHexNewlineEnabled() const
{
    return m_hexNewlineEnabled;
}

void DataProcessor::process(const QByteArray &data)
{
    if (data.isEmpty()) {
        return;
    }

    QString result;

    // 添加时间戳（如果启用）
    // Requirements: 3.2
    if (m_timestampEnabled) {
        result = formatTimestamp() + " >>";
    }

    // 根据格式转换数据
    // Requirements: 3.1
    switch (m_format) {
    case ASCII:
        result += toAsciiString(data);
        break;
    case Hexadecimal:
        result += toHexString(data);
        break;
    }

    // 发出处理完成信号
    // Requirements: 3.3
    emit dataProcessed(result);
}

QString DataProcessor::toHexString(const QByteArray &data) const
{
    // 转换为大写十六进制，每字节用空格分隔
    // 当 m_hexNewlineEnabled 为 true 时，换行符(0x0A)和回车符(0x0D)单独显示并在前后添加换行
    // 当 m_hexNewlineEnabled 为 false 时，0x0A 和 0x0D 作为普通十六进制值显示
    // Requirements: 2.2, 2.3
    QString result;
    result.reserve(data.size() * 4);  // 预分配空间

    for (int i = 0; i < data.size(); ++i) {
        quint8 byte = static_cast<quint8>(data.at(i));
        
        // 处理换行符序列 (0x0D 0x0A 或单独的 0x0A 或 0x0D)
        // Requirements: 2.2 - 当启用时插入换行
        // Requirements: 2.3 - 当禁用时显示为普通十六进制值
        if (m_hexNewlineEnabled && (byte == 0x0A || byte == 0x0D)) {
            if (!result.isEmpty() && !result.endsWith('\n')) {
                result += '\n';
            }
            
            // 收集连续的换行符
            QString controlSequence = "[" + QString("%1").arg(byte, 2, 16, QChar('0')).toUpper();
            
            // 查看后续字节是否也是换行符
            while (i + 1 < data.size()) {
                quint8 nextByte = static_cast<quint8>(data.at(i + 1));
                if (nextByte == 0x0A || nextByte == 0x0D) {
                    controlSequence += " " + QString("%1").arg(nextByte, 2, 16, QChar('0')).toUpper();
                    i++;  // 移动到下一个字节
                } else {
                    break;
                }
            }
            
            controlSequence += "]\n";
            result += controlSequence;
        } else {
            // 普通字节处理（或禁用换行时的 0x0A/0x0D）
            if (!result.isEmpty() && !result.endsWith(' ') && !result.endsWith('\n')) {
                result += ' ';
            }
            result += QString("%1").arg(byte, 2, 16, QChar('0')).toUpper();
        }
    }

    // 确保结果末尾没有多余的空格
    if (result.endsWith(' ')) {
        result.chop(1);
    }

    return result;
}

QString DataProcessor::toAsciiString(const QByteArray &data) const
{
    // 根据当前编码设置转换字节数据
    // Requirements: 1.2
    switch (m_encoding) {
    case AppSettings::UTF8: {
        // 使用 UTF-8 编码转换 (Qt 6 QStringDecoder)
        auto decoder = QStringDecoder(QStringDecoder::Utf8);
        if (decoder.isValid()) {
            QString result = decoder(data);
            return result;
        }
        break;
    }
    case AppSettings::GBK: {
        // 使用 GBK 编码转换 (Qt 6 QStringDecoder)
        auto decoder = QStringDecoder("GBK");
        if (decoder.isValid()) {
            QString result = decoder(data);
            return result;
        }
        // GBK 可能不可用，回退到 System 编码
        auto sysDecoder = QStringDecoder(QStringDecoder::System);
        if (sysDecoder.isValid()) {
            return sysDecoder(data);
        }
        break;
    }
    case AppSettings::ANSI:
    default:
        // 使用 Latin1 编码转换（ANSI/Windows 本地编码）
        return QString::fromLatin1(data);
    }
    
    // 回退到 Latin1
    return QString::fromLatin1(data);
}

QString DataProcessor::formatTimestamp() const
{
    // 格式：HH:mm:ss.zzz
    // Requirements: 3.2
    return QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
}
