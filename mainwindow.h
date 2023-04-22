/****************************************************************************
**
** 主窗口界面类，主要包括菜单栏的设置
**
****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <DMenu>
#include <DMenuBar>
#include <DToolBar>
#include <DLabel>
#include <DScrollArea>
#include "widget.h"
#include "gitsync.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //初始化菜单栏的函数
    void initMenu();

private slots:
    //打开、新建、保存、另存为文件的信号与槽
    void onFileOpen();
    void onFileNew();
    void onFileSave();
    void onFileSaveAs();

protected:
    void closeEvent(QCloseEvent *e) override;

private:
    Widget *w;
    Ui::MainWindow *ui;


    QString url;


};

#endif // MAINWINDOW_H
