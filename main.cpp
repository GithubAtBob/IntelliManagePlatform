#include "login.h"
#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(":/IntelliManagePlatform_zh_CN.qm");
    a.installTranslator(&translator);
    login w;
    w.show();
    return a.exec();
}
