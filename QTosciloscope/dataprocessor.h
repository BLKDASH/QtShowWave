#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QDateTime>
#include "appsettings.h"

/**
 * @brief DataProcessor - 数据处理器
 * 
 * 负责将原始字节数据转换为可显示格式（ASCII 或 Hexadecimal）。
 * 支持时间戳格式化功能。
 * 
 * Requirements: 3.1, 3.2, 3.3
 */
class DataProcessor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 数据显示格式
     */
    enum Format {
        ASCII,          ///< ASCII 文本格式
        Hexadecimal     ///< 十六进制格式
    };
    Q_ENUM(Format)

    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit DataProcessor(QObject *parent = nullptr);

    /**
     * @brief 设置数据显示格式
     * @param format 显示格式（ASCII 或 Hexadecimal）
     * Requirements: 3.1
     */
    void setFormat(Format format);

    /**
     * @brief 获取当前数据显示格式
     * @return 当前格式
     */
    Format format() const;

    /**
     * @brief 设置是否启用时间戳
     * @param enabled true 启用时间戳，false 禁用
     * Requirements: 3.2
     */
    void setTimestampEnabled(bool enabled);

    /**
     * @brief 设置文本编码方式
     * @param encoding 编码方式（ANSI, UTF8, GBK）
     * Requirements: 1.2
     */
    void setEncoding(AppSettings::Encoding encoding);

    /**
     * @brief 获取当前文本编码方式
     * @return 当前编码方式
     */
    AppSettings::Encoding encoding() const;

    /**
     * @brief 设置十六进制显示模式下是否启用换行
     * @param enabled true 启用换行（0A/0D 触发换行），false 禁用
     * Requirements: 2.2, 2.3
     */
    void setHexNewlineEnabled(bool enabled);

    /**
     * @brief 检查十六进制换行是否启用
     * @return true 如果十六进制换行已启用
     */
    bool isHexNewlineEnabled() const;

    /**
     * @brief 检查时间戳是否启用
     * @return true 如果时间戳已启用
     */
    bool isTimestampEnabled() const;

public slots:
    /**
     * @brief 处理原始数据
     * 
     * 将原始字节数据转换为指定格式的字符串。
     * 如果启用时间戳，将在数据前添加时间戳。
     * 处理完成后发出 dataProcessed 信号。
     * 
     * @param data 原始字节数据
     * Requirements: 3.1, 3.2, 3.3
     */
    void process(const QByteArray &data);

signals:
    /**
     * @brief 数据处理完成信号
     * 
     * 当数据转换完成时发出，携带处理后的字符串。
     * Requirements: 3.3
     * 
     * @param text 处理后的可显示字符串
     */
    void dataProcessed(const QString &text);

private:
    /**
     * @brief 将字节数组转换为十六进制字符串
     * 
     * 输出格式：大写十六进制，每字节用空格分隔。
     * 例如：{0x48, 0x65} -> "48 65"
     * 
     * @param data 原始字节数据
     * @return 十六进制字符串
     * Requirements: 3.1
     */
    QString toHexString(const QByteArray &data) const;

    /**
     * @brief 将字节数组转换为 ASCII 字符串
     * 
     * 保留可打印字符，不可打印字符保持原样（由 QString::fromLatin1 处理）。
     * 
     * @param data 原始字节数据
     * @return ASCII 字符串
     * Requirements: 3.1
     */
    QString toAsciiString(const QByteArray &data) const;

    /**
     * @brief 格式化当前时间戳
     * 
     * 格式：HH:mm:ss.zzz
     * 
     * @return 格式化的时间戳字符串
     * Requirements: 3.2
     */
    QString formatTimestamp() const;

    Format m_format = ASCII;           ///< 当前显示格式
    bool m_timestampEnabled = false;   ///< 时间戳启用状态
    AppSettings::Encoding m_encoding = AppSettings::ANSI;  ///< 文本编码方式
    bool m_hexNewlineEnabled = true;   ///< 十六进制换行启用状态 (Requirements: 2.2, 2.3)
};

#endif // DATAPROCESSOR_H
