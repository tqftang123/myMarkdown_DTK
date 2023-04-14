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
#include <QInputDialog>
#include <QMessageBox>


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

            QAction *openFile=new QAction("打开");

            //创建菜单项并连接信号与槽
            //connect(menu->addAction("打开"), &QAction::triggered, this, &MainWindow::onFileOpen);
           menu->addAction(openFile);
           connect(openFile,&QAction::triggered, this, &MainWindow::onFileOpen);

           //快捷键的设置
           //openFile->setShortcut(QKeySequence("Qt::CTRL+Qt::Key_O."));
           //openFile->setShortcut(tr("ctrl+o"));

            connect(menu->addAction("新建"), &QAction::triggered, this, &MainWindow::onFileNew);
            connect(menu->addAction("保存"), &QAction::triggered, this, &MainWindow::onFileSave);
            connect(menu->addAction("另存为"), &QAction::triggered, this, &MainWindow::onFileSaveAs);

            //titlebar->menu()->addAction("markdown指南");
            connect(titlebar->menu()->addAction("markdown指南"), &QAction::triggered, this, [&](){
                w->openFile(":/markdown_guide.md");
                w->view();
            });

            //该软件如何使用的指南，用户使用手册
             titlebar->menu()->addAction("用户使用手册");


             connect(titlebar->menu()->addAction("切换目录路径"), &QAction::triggered, this, [&](){
QString path=QFileDialog::getExistingDirectory(this,tr("选择创建文件路径"),"../");
w->changePath(path);
             });


            //titlebar->menu()->addAction("关于");
            connect(titlebar->menu()->addAction("关于"), &QAction::triggered, this, [&](){
                QString dlgTitle = "关于对话框";
                QString strInfo = "该项目为markdown文本编辑器，开发小组成员有：";
                QMessageBox::about(this,dlgTitle,strInfo);

            });



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
        buttonBox->setFixedWidth(520);
        DButtonBoxButton *db1=new DButtonBoxButton("用户");
        //DButtonBoxButton *db66=new DButtonBoxButton("|");
        DButtonBoxButton *db2=new DButtonBoxButton("新建笔记本");
        DButtonBoxButton *db3=new DButtonBoxButton("新建笔记");
        //DButtonBoxButton *db77=new DButtonBoxButton("|");
        DButtonBoxButton *db4=new DButtonBoxButton("编辑");
        DButtonBoxButton *db5=new DButtonBoxButton("预览");
        DButtonBoxButton *db6=new DButtonBoxButton("导出到");
        DButtonBoxButton *db7=new DButtonBoxButton("保存");
        DButtonBoxButton *db8=new DButtonBoxButton("导入");

        //buttonBox->setButtonList({new DButtonBoxButton("用户"), new DButtonBoxButton("新建笔记本"),new DButtonBoxButton("新建笔记"),new DButtonBoxButton("导入"),new DButtonBoxButton("导出")}, true);
        buttonBox->setButtonList({db1,db2 ,db3,db7,db4,db5,db6,db8}, true);

        //buttonBox->setButtonList({db1,db66,db2 ,db3,db77,db4,db5}, true);

        buttonBox->setId(buttonBox->buttonList().at(0), 0);
        buttonBox->setId(buttonBox->buttonList().at(1), 1);
        titlebar->addWidget(buttonBox);

        connect(db3,&DButtonBoxButton::clicked,this,[=](){
            //现在是指定了目录和文件名的，可以通过弹出对话框来让用户进行选择，也可默认添加到当前目录结构下，文件名还是需要用户指定的
QString path=QFileDialog::getExistingDirectory(this,tr("选择创建文件路径"),"../");
if(path!=nullptr)
{
    QString sName = QInputDialog::getText(this,
                                             "QInputdialog_Name",
                                             "请输入要创建的笔记名称",
                                             QLineEdit::Normal,
                                             "abc"
                                             );
    if(sName!=nullptr)
    {
        w->creatFile(path,sName+".md");
        w->fileSave();
        w->changePath(path);
        w->openFile(path+"/"+sName+".md");
    }
    else {
        QMessageBox::warning(this,tr("创建失败"),tr("未输入笔记名称"));
    }
}
else {
    QMessageBox::warning(this,tr("创建失败"),tr("未选择笔记存储路径"));
}



        });

        connect(db2,&DButtonBoxButton::clicked,this,[=](){

            QString path=QFileDialog::getExistingDirectory(this,tr("选择创建文件路径"),"../");
            if(path!=nullptr)
            {
                QString sName = QInputDialog::getText(this,
                                                         "QInputdialog_Name",
                                                         "请输入要创建的笔记本名称",
                                                         QLineEdit::Normal,
                                                         "abc"
                                                         );
                if(sName!=nullptr)
                {
                    w->creatDir(path,sName);
                    w->changePath(path+"/"+sName);
                }
                else {
                    QMessageBox::warning(this,tr("创建失败"),tr("未输入笔记本名称"));
                }
            }
            else {
                QMessageBox::warning(this,tr("创建失败"),tr("未选择笔记本存储路径"));
            }

        });



        connect(db4,&DButtonBoxButton::clicked,this,[=](){
            w->edite();
        });
        connect(db5,&DButtonBoxButton::clicked,this,[=](){
            w->view();
        });
        connect(db6,&DButtonBoxButton::clicked,this,[=](){
            QString fileName;
            //选择 导出的文件路径
            QString path=QFileDialog::getExistingDirectory(this,tr("选择导出文件保存路径"),"../");
            if(path!=nullptr)
            {
                //输入导出的文件名
                QString sName = QInputDialog::getText(this,
                                                         "QInputdialog_Name",
                                                         "请输入导出文件保存的文件名",
                                                         QLineEdit::Normal,
                                                         "filename"
                                                         );


                if(sName!=nullptr)
                {
                    fileName=path+"/"+sName;

                    // 创建一个询问框，并添加三个自定义按钮
                        QMessageBox msgBox;
                        msgBox.setText("请问导出文件为什么格式?");
                        msgBox.addButton("pdf", QMessageBox::YesRole);
                        msgBox.addButton("word", QMessageBox::NoRole);
                        msgBox.addButton("pdf/word", QMessageBox::RejectRole);
                        msgBox.exec();

                        // 获取用户的选择，并显示相应的消息
                        QAbstractButton *clickedButton = msgBox.clickedButton();
                        if (msgBox.buttonRole(clickedButton) == QMessageBox::YesRole) {
                            //QMessageBox::information(nullptr, "Answer", "pdf");
                            w->coutPdf(fileName);
                        } else if (msgBox.buttonRole(clickedButton) == QMessageBox::NoRole) {
                            //QMessageBox::information(nullptr, "Answer", "word");
                            w->coutWord(fileName);
                        } else if (msgBox.buttonRole(clickedButton) == QMessageBox::RejectRole) {
                            //QMessageBox::information(nullptr, "Answer", "pdf/word");
                            w->coutPdf(fileName);
                            w->coutWord(fileName);
                        }
                }
                else {
                    QMessageBox::warning(this,tr("导出失败"),tr("未输入导出文件名"));
                }
            }
            else {
                 QMessageBox::warning(this,tr("导出失败"),tr("未选择导出文件存储路径"));
            }







        });

        connect(db7,&DButtonBoxButton::clicked,this,[=](){
            w->fileSave();
        });

        connect(db8,&DButtonBoxButton::clicked,this,[=](){
//            QString path=QFileDialog::getExistingDirectory(this,tr("选择导入文件保存路径"),"../");
//            w->cinWord(path);
            QString path=QFileDialog::getOpenFileName(this,tr("选择导入文件保存路径"));
             w->cinWord(path);
        });

        connect(db7,&DButtonBoxButton::clicked,this,[=](){
            w->fileSave();
        });



//设置快捷键
        db5->setShortcut(tr("ctrl+w"));
        //ctrl+v可能用不起，因为ctrl+v本来是粘贴的意思。
        db4->setShortcut(tr("ctrl+e"));

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
