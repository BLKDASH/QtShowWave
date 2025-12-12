#include "widget.h"

#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

// 加载嵌入字体的函数
void loadEmbeddedFonts() {
    int fontId = QFontDatabase::addApplicationFont(":/new/prefix1/recf/HarmonyOS_Sans_SC_Regular.ttf");
    if (fontId != -1) {
        QStringList fontFamilyList = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilyList.isEmpty()) {
            qDebug() << "成功加载字体:" << fontFamilyList.at(0);
            // 可以在这里设置应用程序使用该字体
            QFont customFont(fontFamilyList.at(0), 9);
            QApplication::setFont(customFont);
        }
    } else {
        qDebug() << "字体加载失败，请检查资源文件路径";
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loadEmbeddedFonts();
    Widget w;
    w.show();

    return a.exec();
}
