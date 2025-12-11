#include "speedmonitor.h"

SpeedMonitor::SpeedMonitor(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_totalBytes(0)
    , m_intervalBytes(0)
    , m_currentSpeed(0.0)
{
    m_timer->setInterval(UPDATE_INTERVAL_MS);
    connect(m_timer, &QTimer::timeout, this, &SpeedMonitor::onTimeout);
}

SpeedMonitor::~SpeedMonitor()
{
    stop();
}

void SpeedMonitor::recordBytes(qint64 bytes)
{
    if (bytes > 0) {
        m_totalBytes += bytes;
        m_intervalBytes += bytes;
    }
}

void SpeedMonitor::reset()
{
    m_totalBytes = 0;
    m_intervalBytes = 0;
    m_currentSpeed = 0.0;
}

void SpeedMonitor::start()
{
    m_timer->start();
}

void SpeedMonitor::stop()
{
    m_timer->stop();
    m_currentSpeed = 0.0;
}

qint64 SpeedMonitor::totalBytes() const
{
    return m_totalBytes;
}

double SpeedMonitor::currentSpeed() const
{
    return m_currentSpeed;
}

void SpeedMonitor::onTimeout()
{
    // Calculate speed based on bytes received in this interval
    m_currentSpeed = static_cast<double>(m_intervalBytes);
    m_intervalBytes = 0;
    
    emit speedUpdated(m_currentSpeed, m_totalBytes);
}

QString SpeedMonitor::formatSpeed(double bytesPerSecond)
{
    if (bytesPerSecond >= 1024.0) {
        return QString("%1 KB/s").arg(bytesPerSecond / 1024.0, 0, 'f', 1);
    }
    return QString("%1 bytes/s").arg(static_cast<int>(bytesPerSecond));
}

QString SpeedMonitor::formatBytes(qint64 bytes)
{
    if (bytes >= 1048576) {
        return QString("%1 MB").arg(static_cast<double>(bytes) / 1048576.0, 0, 'f', 1);
    }
    if (bytes >= 1024) {
        return QString("%1 KB").arg(static_cast<double>(bytes) / 1024.0, 0, 'f', 1);
    }
    return QString("%1 B").arg(bytes);
}
