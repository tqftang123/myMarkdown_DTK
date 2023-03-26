#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <DMainWindow>
#include <DWidgetUtil> //加入此头文件方可使用moveToCenter

#include <DDialog>
#include <DButtonBox>
#include <DTitlebar>
#include "dtkwidget_global.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //有ui设计文件，但里面没有东西
    ui->setupUi(this);

    //在设计师里面添加了菜单栏之后，dtk自带的菜单栏就没有了，最好用代码添加菜单栏，工具栏什么的
    //初始化主窗口 Initialize the main window
    moveToCenter(this); //把窗口移动到屏幕中间
    //MainWindow::resize(1330,1200); //改变窗口大小应当改变MainWindow的大小

    initMenu();


    //中间的内容部分，调用自定义的Widget类
    w = new Widget;
    //resize(w->size()); //设置窗口大小
    setCentralWidget(w);


}

MainWindow::~MainWindow()
{

}



//设置菜单栏
void MainWindow::initMenu()
{
    //DMenuBar *menuBar= this->menuBar();
//    fileMenu=new QMenu(tr("&file"),this);
//    openAct=new QAction(tr("&Open"),this);
//    fileMenu->addAction(openAct);
    //this->menuBar()->addMenu(fileMenu);


//DTitlebar类，dtk自定义的菜单栏
    DTitlebar *titlebar = this->titlebar();
        if (titlebar) {
            titlebar->setIcon(QIcon(""));
            titlebar->setMenu(new QMenu(titlebar));
            titlebar->setSeparatorVisible(true);
            QMenu *menu = titlebar->menu()->addMenu("文件");
//            menu->addAction("新建");
//            menu->addAction("打开");
//            menu->addAction("保存");
//            menu->addAction("另存为");
            //创建菜单项并连接信号与槽
            connect(menu->addAction("打开"), &QAction::triggered, this, &MainWindow::onFileOpen);
            connect(menu->addAction("新建"), &QAction::triggered, this, &MainWindow::onFileNew);
            connect(menu->addAction("保存"), &QAction::triggered, this, &MainWindow::onFileSave);
            connect(menu->addAction("另存为"), &QAction::triggered, this, &MainWindow::onFileSaveAs);

            titlebar->menu()->addAction("快捷键");
            titlebar->menu()->addAction("帮助");
            titlebar->menu()->addAction("markdown指南");

//           QMenu *menu = titlebar->menu()->addMenu("sub-menu");
//            connect(menu->addAction("show full screen"), &QAction::triggered, this, [this]() {
//                this->isFullScreen() ? this->showNormal() : this->showFullScreen();
//                if (QAction *action = qobject_cast<QAction *>(sender())) {
//                    action->setText(this->isFullScreen() ? "show normal window" : "show full screen");
//                }
//            });
//            connect(menu->addAction("ddialog"), &QAction::triggered, this, []() {
//                DDialog dlg("this is title", "this is message text......");
//                dlg.addButton("ok", true, DDialog::ButtonWarning);
//                dlg.setIcon(QIcon::fromTheme("dialog-information"));
//                dlg.exec();
//            });
            //connect(titlebar->menu(), &QMenu::triggered, this, &MainWindow::menuItemInvoked);

            titlebar->setDisableFlags(Qt::WindowMinimizeButtonHint
                                      | Qt::WindowMaximizeButtonHint
                                      | Qt::WindowSystemMenuHint);
            titlebar->setAutoHideOnFullscreen(true);
        }



        DButtonBox *buttonBox = new DButtonBox(titlebar);
        buttonBox->setFixedWidth(370);
        buttonBox->setButtonList({new DButtonBoxButton("用户"), new DButtonBoxButton("新建笔记本"),new DButtonBoxButton("新建笔记"),new DButtonBoxButton("导入"),new DButtonBoxButton("导出")}, true);
        buttonBox->setId(buttonBox->buttonList().at(0), 0);
        buttonBox->setId(buttonBox->buttonList().at(1), 1);
        titlebar->addWidget(buttonBox);


}



void MainWindow::onFileOpen()
{
    if (w->isModified()) {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to open a new document anyway?"));
        if (button != QMessageBox::Yes)
            return;
    }
    QFileDialog dialog(this, tr("Open MarkDown File"));
    dialog.setMimeTypeFilters({"text/markdown"});
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() == QDialog::Accepted)
        w->openFile(dialog.selectedFiles().constFirst());
}



void MainWindow::onFileNew()
{
    w->fileNew();
}


void MainWindow::onFileSave()
{
    w->fileSave();
}

void MainWindow::onFileSaveAs()
{
   w->fileSaveAs();
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    if (w->isModified()) {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to exit anyway?"));
        if (button != QMessageBox::Yes) {
            e->ignore();
        }
    }
}
