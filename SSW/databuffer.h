#ifndef DATABUFFER_H
#define DATABUFFER_H

#include <QObject>
#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>

/**
 * @brief DataBuffer - 线程安全的环形数据缓冲区
 * 
 * 用于在生产者（串口工作线程）和消费者（主UI线程）之间传递数据。
 * 使用 QMutex 保护并发访问，确保数据完整性。
 * 
 * Requirements: 2.1, 2.2, 2.3, 2.4
 */
class DataBuffer : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param capacity 缓冲区最大容量（字节），默认 65536
     * @param parent 父对象
     */
    explicit DataBuffer(int capacity = 65536, QObject *parent = nullptr);

    /**
     * @brief 写入数据到缓冲区
     * 
     * 如果写入后超过容量，将覆盖最旧的数据（保留最新数据）。
     * 线程安全。
     * 
     * @param data 要写入的数据
     * Requirements: 2.1, 2.2, 2.3
     */
    void write(const QByteArray &data);

    /**
     * @brief 读取并清空缓冲区中的所有数据
     * 
     * 返回数据按 FIFO 顺序排列。
     * 线程安全。
     * 
     * @return 缓冲区中的所有数据
     * Requirements: 2.3, 2.4
     */
    QByteArray readAll();

    /**
     * @brief 检查缓冲区是否为空
     * 
     * 线程安全。
     * 
     * @return true 如果缓冲区为空
     */
    bool isEmpty() const;

    /**
     * @brief 获取缓冲区当前数据大小
     * 
     * 线程安全。
     * 
     * @return 当前存储的字节数
     */
    int size() const;

    /**
     * @brief 清空缓冲区
     * 
     * 线程安全。
     */
    void clear();

    /**
     * @brief 获取缓冲区容量
     * @return 最大容量（字节）
     */
    int capacity() const;

private:
    QByteArray m_buffer;      ///< 数据存储
    int m_capacity;           ///< 最大容量
    mutable QMutex m_mutex;   ///< 互斥锁，保护并发访问
};

#endif // DATABUFFER_H
