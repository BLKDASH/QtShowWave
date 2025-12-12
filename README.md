# SSW Serial Helper

一款基于 Qt6 开发的串口调试助手，界面简洁，功能实用。

## 功能特性

### 串口通信
- 串口自动检测与刷新
- 支持常用波特率（115200 等）
- 数据位、停止位、校验位可配置
- 独立工作线程，UI 不阻塞
- 自动重连上次使用的串口

### 数据收发
- ASCII / 十六进制双模式显示
- 实时速度监控（bytes/s）与累计统计
- 时间戳显示（HH:mm:ss.zzz）
- 自动滚动 / 手动暂停
- 关键词高亮（DEBUG/INFO/WARN/ERROR/FATAL）
- 十六进制模式 0A/0D 换行支持
- 普通文本 / 十六进制发送
- 自动追加换行符、发送后清空

### 编码支持
- ANSI / UTF-8 / GBK

### 界面设置
- 深色模式
- 字体大小调节（6-24pt）
- 多字体选择（系统默认 / HarmonyOS Sans SC / Alibaba PuHuiTi / MiSans）
- 窗口大小与分割条位置记忆
- 设置自动持久化

## 构建

### 环境要求
- Qt 6.x
- C++17
- Qt Serial Port 模块

### 编译步骤
```bash
qmake SSW.pro
make
```

或使用 Qt Creator 打开 `SSW.pro` 直接构建。

## 下载

预编译版本：`SSW_SerialHelper_Win_x64_vX_X_X_Portable.zip`

## 截图

（待添加）

## 版权所有

© 2025 tubohu-广工电子科技协会嵌入式组
