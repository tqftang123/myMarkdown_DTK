#include "mainwindow.h"
#include <DApplication>
#include <DMainWindow>
#include "widget.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    DApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    w.show();

    return a.exec();
}
