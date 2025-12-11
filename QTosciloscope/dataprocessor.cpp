#include "dataprocessor.h"

/**
 * @brief DataProcessor 实现
 * 
 * Requirements: 3.1, 3.2, 3.3
 */

DataProcessor::DataProcessor(QObject *parent)
    : QObject(parent)
    , m_format(ASCII)
    , m_timestampEnabled(false)
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
    // 换行符(0x0A)和回车符(0x0D)单独显示并在前后添加换行
    QString result;
    result.reserve(data.size() * 4);  // 预分配空间

    for (int i = 0; i < data.size(); ++i) {
        quint8 byte = static_cast<quint8>(data.at(i));
        
        // 处理换行符序列 (0x0D 0x0A 或单独的 0x0A 或 0x0D)
        if (byte == 0x0A || byte == 0x0D) {
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
            // 普通字节处理
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
    // 使用 Latin1 编码转换，保留所有字节值
    return QString::fromLatin1(data);
}

QString DataProcessor::formatTimestamp() const
{
    // 格式：HH:mm:ss.zzz
    // Requirements: 3.2
    return QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
}
