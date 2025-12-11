#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <QSerialPortInfo>
#include <QRegularExpression>

/**
 * @brief Widget 构造函数
 * 
 * 初始化所有组件并建立信号槽连接。
 * Requirements: 1.3, 6.1, 6.2, 6.3, 6.4
 */
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_worker(new SerialWorker(this))
    , m_processor(new DataProcessor(this))
    , m_buffer(new DataBuffer(65536, this))
    , m_refreshTimer(new QTimer(this))
    , m_autoScroll(true)
{
    ui->setupUi(this);

    ui->open->setStyleSheet("color: red;");
    ui->cbBaudRate->setCurrentText("115200");

    QList<int> sizes;
    sizes << 30000 << 50000;
    ui->splitter->setSizes(sizes);

    ui->sbFontSize->setValue(9);

    updatePortList();
    setupConnections();

    m_refreshTimer->setTimerType(Qt::PreciseTimer);
    m_refreshTimer->setInterval(REFRESH_INTERVAL_MS);
}

Widget::~Widget()
{
    m_refreshTimer->stop();
    if (m_worker->isRunning()) {
        m_worker->stop();
    }
    delete ui;
}

/**
 * @brief 建立所有信号槽连接
 */
void Widget::setupConnections()
{
    connect(m_worker, &SerialWorker::dataReceived, this, &Widget::onDataReceived);
    connect(m_worker, &SerialWorker::errorOccurred, this, &Widget::onSerialError);
    connect(m_worker, &SerialWorker::started, this, &Widget::onSerialStarted);
    connect(m_worker, &SerialWorker::stopped, this, &Widget::onSerialStopped);

    connect(m_processor, &DataProcessor::dataProcessed, this, &Widget::onDataProcessed);

    connect(m_refreshTimer, &QTimer::timeout, this, &Widget::onRefreshTimeout);

    QScrollBar *scrollBar = ui->receiveEdit->verticalScrollBar();
    connect(scrollBar, &QScrollBar::valueChanged, this, &Widget::onScrollValueChanged);
}


/**
 * @brief 更新可用串口列表
 */
void Widget::updatePortList()
{
    QString lastPortName = ui->cbPortName->currentText();
    ui->cbPortName->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->cbPortName->addItem(info.portName());
    }

    if (ui->cbPortName->findText(lastPortName) >= 0) {
        ui->cbPortName->setCurrentText(lastPortName);
    }
}

/**
 * @brief 设置串口配置控件的启用状态
 * @param enabled true 启用，false 禁用
 */
void Widget::setPortControlsEnabled(bool enabled)
{
    ui->cbBaudRate->setEnabled(enabled);
    ui->cbDataBits->setEnabled(enabled);
    ui->cbParity->setEnabled(enabled);
    ui->cbPortName->setEnabled(enabled);
    ui->cbStopBits->setEnabled(enabled);
}

/**
 * @brief 从UI控件构建串口配置
 * @return SerialConfig 配置结构
 */
SerialConfig Widget::buildConfig() const
{
    SerialConfig config;
    config.portName = ui->cbPortName->currentText();
    config.baudRate = ui->cbBaudRate->currentText().toInt();

    switch (ui->cbStopBits->currentIndex()) {
        case 0: config.stopBits = QSerialPort::OneStop; break;
        case 1: config.stopBits = QSerialPort::OneAndHalfStop; break;
        case 2: config.stopBits = QSerialPort::TwoStop; break;
        default: config.stopBits = QSerialPort::OneStop; break;
    }

    switch (ui->cbParity->currentIndex()) {
        case 0: config.parity = QSerialPort::NoParity; break;
        case 1: config.parity = QSerialPort::OddParity; break;
        case 2: config.parity = QSerialPort::EvenParity; break;
        default: config.parity = QSerialPort::NoParity; break;
    }

    switch (ui->cbDataBits->currentIndex()) {
        case 0: config.dataBits = QSerialPort::Data8; break;
        case 1: config.dataBits = QSerialPort::Data7; break;
        case 2: config.dataBits = QSerialPort::Data6; break;
        case 3: config.dataBits = QSerialPort::Data5; break;
        default: config.dataBits = QSerialPort::Data8; break;
    }

    config.flowControl = QSerialPort::NoFlowControl;
    config.readBufferSize = 4096;

    return config;
}

/**
 * @brief 追加文本到显示区域
 * 
 * 根据自动滚动状态决定是否滚动到底部。
 * Requirements: 6.4
 * 
 * @param text 要追加的文本
 */
void Widget::appendToDisplay(const QString &text)
{
    ui->receiveEdit->insertPlainText(text);

    if (m_autoScroll) {
        ui->receiveEdit->moveCursor(QTextCursor::End);
    }
}


/**
 * @brief 定时刷新回调
 * 
 * 批量处理缓冲区中的数据并更新UI。
 * Requirements: 6.1, 6.2, 6.3
 */
void Widget::onRefreshTimeout()
{
    if (!m_pendingText.isEmpty()) {
        appendToDisplay(m_pendingText);
        m_pendingText.clear();
    }
}

/**
 * @brief 处理接收到的串口数据
 * 
 * 将数据写入缓冲区并交给处理器处理。
 * 
 * @param data 接收到的原始数据
 */
void Widget::onDataReceived(const QByteArray &data)
{
    m_buffer->write(data);

    m_processor->setFormat(ui->chk0x16Show->isChecked() 
        ? DataProcessor::Hexadecimal 
        : DataProcessor::ASCII);
    m_processor->setTimestampEnabled(ui->chkTimeShow->isChecked());
    m_processor->process(data);
}

/**
 * @brief 处理已处理的数据
 * 
 * 将处理后的文本添加到待显示队列。
 * Requirements: 6.1, 6.2
 * 
 * @param text 处理后的文本
 */
void Widget::onDataProcessed(const QString &text)
{
    m_pendingText.append(text);
}

/**
 * @brief 处理串口错误
 * @param error 错误描述
 */
void Widget::onSerialError(const QString &error)
{
    QMessageBox::warning(this, "串口错误", error, QMessageBox::Ok);
}

/**
 * @brief 显示系统消息
 * 
 * 统一显示系统相关信息，添加"SystInfo >>"前缀
 * 
 * @param message 系统消息内容
 */
void Widget::showSystemMessage(const QString &message)
{
    ui->receiveEdit->appendPlainText("SysInfo >> " + message);
}

/**
 * @brief 串口启动成功回调
 */
void Widget::onSerialStarted()
{
    setPortControlsEnabled(false);
    ui->receiveEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->open->setText("关闭串口");
    ui->open->setStyleSheet("color: orange;");
    ui->lbConnected->setText("当前已连接");
    ui->lbConnected->setStyleSheet("color: green;");
    showSystemMessage("串口已连接！\r\n");

    m_refreshTimer->start();
}

/**
 * @brief 串口停止回调
 */
void Widget::onSerialStopped()
{
    m_refreshTimer->stop();

    if (!m_pendingText.isEmpty()) {
        appendToDisplay(m_pendingText);
        m_pendingText.clear();
    }

    setPortControlsEnabled(true);
    ui->receiveEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    ui->open->setText("打开串口");
    ui->open->setStyleSheet("color: red;");
    ui->lbConnected->setText("当前未连接");
    ui->lbConnected->setStyleSheet("color: rgb(0, 85, 255);");
    showSystemMessage("串口已关闭！\r\n");
}

/**
 * @brief 滚动条值变化回调
 * 
 * 实现自动滚动暂停/恢复逻辑。
 * Requirements: 6.4
 * 
 * @param value 当前滚动条值
 */
void Widget::onScrollValueChanged(int value)
{
    QScrollBar *scrollBar = ui->receiveEdit->verticalScrollBar();
    m_autoScroll = (value >= scrollBar->maximum() - 10);
}


/**
 * @brief 清空接收区
 */
void Widget::on_clear_clicked()
{
    ui->receiveEdit->clear();
    m_pendingText.clear();
    m_buffer->clear();
}

/**
 * @brief 检测串口按钮点击
 */
void Widget::on_cbPortName_clicked()
{
    updatePortList();
    showSystemMessage("检测端口完毕\r\n");
}

/**
 * @brief 打开/关闭串口按钮点击
 */
void Widget::on_open_clicked()
{
    if (!m_worker->isRunning()) {
        SerialConfig config = buildConfig();

        if (!config.isValid()) {
            QMessageBox::warning(this, "配置错误", config.validationError(), QMessageBox::Ok);
            return;
        }

        m_worker->start(config);
    } else {
        m_worker->stop();
    }
}

/**
 * @brief 发送按钮点击
 */
void Widget::on_send_clicked()
{
    if (!m_worker->isRunning()) {
        QMessageBox::information(this, "提示", "串口未打开！", QMessageBox::Ok);
        return;
    }

    QByteArray sendData = ui->sendEdit->toPlainText().toLocal8Bit();
    if (sendData.isEmpty()) {
        return;
    }

    QString line = "\r\n-------------------------------------\r\n";
    QString showTheSend = "send>>>  " + ui->sendEdit->toPlainText() + "  >>>end\r\n";
    ui->receiveEdit->appendPlainText(line);
    ui->receiveEdit->appendPlainText(showTheSend);

    if (ui->chk0x16Send->isChecked()) {
        static QRegularExpression hexRegex("[A-Fa-f0-9]{2}");
        QString strData = ui->sendEdit->toPlainText();
        if (!hexRegex.match(strData).hasMatch()) {
            QMessageBox::information(this, "提示", "请输入16进制数", QMessageBox::Ok);
            return;
        }
        sendData = QByteArray::fromHex(sendData);
    }

    if (ui->chkNewLine->isChecked()) {
        sendData.append("\r\n");
    }

    m_worker->sendData(sendData);

    if (ui->chkClearAfterSend->isChecked()) {
        ui->sendEdit->clear();
    }

    ui->sendEdit->setFocus();
}

/**
 * @brief 清空发送区按钮点击
 */
void Widget::on_clearSend_clicked()
{
    ui->sendEdit->clear();
}

/**
 * @brief 字体大小调整
 * @param arg1 新的字体大小
 */
void Widget::on_sbFontSize_valueChanged(int arg1)
{
    QFont font;
    font.setPointSize(arg1);
    ui->receiveEdit->setFont(font);
    ui->sendEdit->setFont(font);
}
