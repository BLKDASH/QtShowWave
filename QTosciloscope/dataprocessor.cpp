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
        result = formatTimestamp() + " ";
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
    // 例如：{0x48, 0x65, 0x6C} -> "48 65 6C"
    QString result;
    result.reserve(data.size() * 3);  // 预分配空间：每字节2个字符 + 1个空格

    for (int i = 0; i < data.size(); ++i) {
        if (i > 0) {
            result += ' ';
        }
        result += QString("%1").arg(static_cast<unsigned char>(data.at(i)), 2, 16, QChar('0')).toUpper();
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
