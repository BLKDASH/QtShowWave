#include "widget.h"
#include "appsettings.h"

#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include <QMap>

// 全局字体映射表
static QMap<QString, QString> g_fontFamilyMap;

// 加载所有嵌入字体
void loadAllEmbeddedFonts() {
    struct FontInfo {
        QString path;
        QString displayName;
    };
    
    QList<FontInfo> fonts = {
        {":/new/prefix1/recf/HarmonyOS_Sans_SC_Regular.ttf", "HarmonyOS Sans SC"},
        {":/new/prefix1/recf/AlibabaPuHuiTi-3-45-Light.ttf", "Alibaba PuHuiTi"},
        {":/new/prefix1/recf/MiSans-Normal.ttf", "MiSans"}
    };
    
    for (const auto &font : fonts) {
        int fontId = QFontDatabase::addApplicationFont(font.path);
        if (fontId != -1) {
            QStringList families = QFontDatabase::applicationFontFamilies(fontId);
            if (!families.isEmpty()) {
                g_fontFamilyMap[font.displayName] = families.at(0);
                qDebug() << "成功加载字体:" << font.displayName << "->" << families.at(0);
            }
        } else {
            qDebug() << "字体加载失败:" << font.path;
        }
    }
}

// 保存系统默认字体
static QFont g_systemDefaultFont;

// 应用字体设置
void applyFont(const QString &displayName, int fontSize) {
    QFont customFont;
    if (displayName == "系统默认") {
        // 使用系统默认字体
        customFont = g_systemDefaultFont;
        customFont.setPointSize(fontSize);
        // qDebug() << "应用系统默认字体:" << customFont.family() << "大小:" << fontSize;
    } else {
        QString actualFamily = g_fontFamilyMap.value(displayName, displayName);
        customFont = QFont(actualFamily, fontSize);
        // qDebug() << "应用字体:" << actualFamily << "大小:" << fontSize;
    }
    QApplication::setFont(customFont);
    
    // 强制刷新所有顶层窗口的字体
    for (QWidget *widget : QApplication::topLevelWidgets()) {
        widget->setFont(customFont);
        // 递归更新所有子控件
        for (QWidget *child : widget->findChildren<QWidget*>()) {
            child->setFont(customFont);
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 保存系统默认字体（在加载任何自定义字体之前）
    g_systemDefaultFont = QApplication::font();
    
    // 加载所有嵌入字体
    loadAllEmbeddedFonts();
    
    // 从设置中获取字体配置并应用
    AppSettings *settings = AppSettings::instance();
    // qDebug() << "启动时读取设置 - 字体:" << settings->fontFamily() << "大小:" << settings->fontSize();
    applyFont(settings->fontFamily(), settings->fontSize());
    
    // 连接字体变更信号
    QObject::connect(settings, &AppSettings::fontFamilyChanged, [settings](const QString &family) {
        applyFont(family, settings->fontSize());
    });
    QObject::connect(settings, &AppSettings::fontSizeChanged, [settings](int size) {
        applyFont(settings->fontFamily(), size);
    });
    
    Widget w;
    
    // Widget 创建后再次应用字体，确保所有控件都使用正确的字体
    applyFont(settings->fontFamily(), settings->fontSize());
    
    w.show();

    return a.exec();
}
