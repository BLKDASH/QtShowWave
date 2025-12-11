#include "databuffer.h"

/**
 * @brief DataBuffer - 线程安全的环形数据缓冲区实现
 * 
 * Requirements: 2.1, 2.2, 2.3, 2.4
 */

DataBuffer::DataBuffer(int capacity, QObject *parent)
    : QObject(parent)
    , m_capacity(capacity > 0 ? capacity : 65536)
{
    m_buffer.reserve(m_capacity);
}

void DataBuffer::write(const QByteArray &data)
{
    if (data.isEmpty()) {
        return;
    }

    QMutexLocker locker(&m_mutex);

    // 如果新数据本身就超过容量，只保留最新的 capacity 字节
    if (data.size() >= m_capacity) {
        m_buffer = data.right(m_capacity);
        return;
    }

    // 追加新数据
    m_buffer.append(data);

    // 如果超过容量，移除最旧的数据（保留最新数据）
    // Requirements: 2.2 - 覆盖最旧数据，保留最新数据
    if (m_buffer.size() > m_capacity) {
        int excess = m_buffer.size() - m_capacity;
        m_buffer.remove(0, excess);
    }
}

QByteArray DataBuffer::readAll()
{
    QMutexLocker locker(&m_mutex);

    // Requirements: 2.4 - FIFO 顺序返回数据
    QByteArray result = m_buffer;
    m_buffer.clear();
    return result;
}

bool DataBuffer::isEmpty() const
{
    QMutexLocker locker(&m_mutex);
    return m_buffer.isEmpty();
}

int DataBuffer::size() const
{
    QMutexLocker locker(&m_mutex);
    return m_buffer.size();
}

void DataBuffer::clear()
{
    QMutexLocker locker(&m_mutex);
    m_buffer.clear();
}

int DataBuffer::capacity() const
{
    // capacity 在构造后不变，无需加锁
    return m_capacity;
}
