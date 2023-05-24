#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gitsync.h"

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
#include <QVBoxLayout>
#include <QProcess>

#include <QTimer>


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

    //自定义的初始化菜单栏的函数
    initMenu();


    //中间的内容部分，调用自定义的Widget类
    w = new Widget;
    //resize(w->size()); //设置窗口大小
    setCentralWidget(w);





    // 产生一个定时器
    QTimer *timer = new QTimer(this);

    //连接这个定时器的信号和槽，利用定时器的timeout(),从而触发clear()槽函数
    connect(timer, &QTimer::timeout, this, [=](){
        w->fileSave();
    });

    //开始定时器，并设定定时周期,每隔5秒就会重启定时器，会重复触发定时，除非你利用stop()将定时器关掉
    timer->start(5000);
}

MainWindow::~MainWindow()
{

}



//初始化菜单栏
void MainWindow::initMenu()
{
    //DTitlebar类，dtk自定义的菜单栏
    DTitlebar *titlebar = this->titlebar();
        if (titlebar) {
            titlebar->setIcon(QIcon(""));
            titlebar->setMenu(new QMenu(titlebar));
            titlebar->setSeparatorVisible(true);
            QMenu *menu = titlebar->menu()->addMenu("文件");
            //创建菜单项并连接信号与槽(等价于下面三行代码)(为了设置快捷键所有替换成了后面3行代码）
            //connect(menu->addAction("打开"), &QAction::triggered, this, &MainWindow::onFileOpen);
            QAction *openFile=new QAction("打开");
            menu->addAction(openFile);
            connect(openFile,&QAction::triggered, this, &MainWindow::onFileOpen);

           //快捷键的设置
           //openFile->setShortcut(QKeySequence("Qt::CTRL+Qt::Key_O."));
           //openFile->setShortcut(tr("ctrl+o"));

            //connect(menu->addAction("新建"), &QAction::triggered, this, &MainWindow::onFileNew);
            connect(menu->addAction("保存"), &QAction::triggered, this, &MainWindow::onFileSave);
            connect(menu->addAction("另存为"), &QAction::triggered, this, &MainWindow::onFileSaveAs);

            ////////
            menu = titlebar->menu()->addMenu("同步");
            connect(menu->addAction("设置远程仓库路径"), &QAction::triggered, this, [=](){

                this->url = QInputDialog::getText(this,
                                                             "QInputdialog_Name",
                                                             "请输入github仓库地址",
                                                             QLineEdit::Normal,
                                                             "note"
                                                             );


                ////////////
                w->url2=this->url;

                if(url!=nullptr)
                {
                    // 执行git clone命令，将远程仓库克隆到本地
                    QProcess process;
                    process.start("git clone " + url);
                    process.waitForFinished(-1);

                    // 检查git clone命令是否执行成功
                    if (process.exitCode() != 0)
                    {
                        QMessageBox::warning(this,tr("失败"),tr("同步仓库路径错误！"));
                    }
                    else {
                        QMessageBox::warning(this,tr("同步仓库路径成功！"),tr("仓库已同步至本地！"));


                        QString path=QDir::currentPath();
                        QStringList list = url.split(QRegExp("[/\\\\]"));
                        QString repoName = list.last().remove(".git");
                        QDir::setCurrent("./"+repoName);
                        QString path1=QDir::currentPath();
                        w->changePath(path1);

                    }

                }
                //添加取消和关闭的判断
//                else if(){

//                }
                else {
                    //QMessageBox::warning(this,tr("仓库地址为空"),tr("请输入正确的仓库地址！"));
                }


            });

            connect(menu->addAction("下载"), &QAction::triggered, this, [=](){

                QMessageBox msgBox;
                msgBox.setText("提示：");
                msgBox.setInformativeText("确认下载:"+url+"该仓库地址内容？");
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Ok);
                int ret = msgBox.exec();
                if(ret == QMessageBox::Ok){
                    //...
                    // 执行git pull命令，将本地仓库与远程仓库同步
                    QProcess process;

                    /////// 进入指定文件夹
                        //QDir::setCurrent("./testGit");
                    QString str = this->url;
                    QStringList list = str.split(QRegExp("[/\\\\]"));
                    QString repoName = list.last().remove(".git");
                    qDebug() << repoName;

                    QDir::setCurrent("./"+repoName);
                    process.start("git pull");
                    process.waitForFinished(-1);

                // 检查git pull命令是否执行成功
                    if (process.exitCode() != 0)
                    {
                        //m_statusLabel->setText(tr("Failed to sync to local."));
                        QMessageBox::warning(this,tr("失败"),tr("同步到本地失败！"));
                    }
                    else {
                        QMessageBox::warning(this,tr("成功"),tr("同步到本地成功！"));
                        //m_statusLabel->setText(tr("Successfully synced to local."));
                    }

                }


                if(ret == QMessageBox::Cancel){

                }

            });


            connect(menu->addAction("上传"), &QAction::triggered, this, [=](){

                QMessageBox msgBox;
                msgBox.setText("提示：");
                msgBox.setInformativeText("确认上传到:"+url+"远程仓库？");
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Ok);
                int ret = msgBox.exec();
                if(ret == QMessageBox::Ok){

                    // 执行git add命令，将本地文件添加到暂存区
                    QProcess process;

                    /////// 进入指定文件夹
                        QString str = this->url;
                        QStringList list = str.split(QRegExp("[/\\\\]"));
                        QString repoName = list.last().remove(".git");
                        qDebug() << repoName;
                        QDir::setCurrent("./"+repoName);

                    process.start("git add .");  // 添加所有文件到暂存区
                    process.waitForFinished(-1);

                    // 检查git add命令是否执行成功
                    if (process.exitCode() != 0)
                    {
                        QMessageBox::warning(this,tr("失败"),tr("添加文件到仓库失败！"));
                        //m_statusLabel->setText(tr("Failed to add files."));

                    }
                    else {
                        // 执行git commit命令，提交更改到本地仓库
                        process.start("git commit -m \"Sync changes\"");
                        process.waitForFinished(-1);

                        // 检查git commit命令是否执行成功
                        if (process.exitCode() != 0)
                        {
                            QMessageBox::warning(this,tr("失败"),tr("提交文件到仓库失败！"));
                            //m_statusLabel->setText(tr("Failed to commit changes."));
                        }
                        else {
                            // 执行git push命令，将本地仓库的更改推送到远程仓库
                            process.start("git push");

                            process.waitForFinished(-1);

                            // 检查git push命令是否执行成功
                            if (process.exitCode() != 0)
                            {
                                QMessageBox::warning(this,tr("失败"),tr("上传到远程失败！"));
                                //m_statusLabel->setText(tr("Failed to push changes."));
                            }

                            else {
                                QMessageBox::warning(this,tr("成功"),tr("上传到远程成功！"));
                                //m_statusLabel->setText(tr("Successfully synced to remote."));
                            }

                        }

                    }

                }

                if(ret == QMessageBox::Cancel){

                }

            });

            //titlebar->menu()->addAction("markdown指南");
            connect(titlebar->menu()->addAction("markdown指南"), &QAction::triggered, this, [&](){
                w->openFile(":/markdown_guide.md");
                w->view();
            });

            //该软件如何使用的指南，用户使用手册
             //titlebar->menu()->addAction("用户使用手册");

             connect(titlebar->menu()->addAction("用户使用手册"),&QAction::triggered, this, [&](){
                 w->openFile(":/resources/User.md");
                 w->view();
             });

             connect(titlebar->menu()->addAction("切换目录路径"), &QAction::triggered, this, [&](){
QString path=QFileDialog::getExistingDirectory(this,tr("选择创建文件路径"),"../");
w->changePath(path);
             });

            //titlebar->menu()->addAction("关于");
            connect(titlebar->menu()->addAction("关于"), &QAction::triggered, this, [&](){
                QString dlgTitle = "关于对话框";
                QString strInfo = "该项目为markdown文本编辑器，开发小组成员有：杨东、胡露、唐琴凤。若您在使用中遇到如何问题，欢迎联系我们。邮箱：2805952976@qq.com";
                QMessageBox::about(this,dlgTitle,strInfo);

            });

            titlebar->setDisableFlags(Qt::WindowMinimizeButtonHint
                                      | Qt::WindowMaximizeButtonHint
                                      | Qt::WindowSystemMenuHint);
            titlebar->setAutoHideOnFullscreen(true);
        }

        DButtonBox *buttonBox = new DButtonBox(titlebar);
        //设置菜单栏固定大小
        buttonBox->setFixedWidth(520);
        DButtonBoxButton *db2=new DButtonBoxButton("新建笔记本");
        DButtonBoxButton *db3=new DButtonBoxButton("新建笔记");
        DButtonBoxButton *db4=new DButtonBoxButton("编辑");
        DButtonBoxButton *db5=new DButtonBoxButton("预览");
        DButtonBoxButton *db6=new DButtonBoxButton("导出到");
        DButtonBoxButton *db7=new DButtonBoxButton("保存");
        DButtonBoxButton *db8=new DButtonBoxButton("导入");

        buttonBox->setButtonList({db2 ,db3,db7,db4,db5,db6,db8}, true);
        buttonBox->setId(buttonBox->buttonList().at(0), 0);
        buttonBox->setId(buttonBox->buttonList().at(1), 1);
        titlebar->addWidget(buttonBox);



//        connect(db1,&DButtonBoxButton::clicked,this,[=](){
//            QWidget *mainWindow = new QWidget;
//            QVBoxLayout *mainLayout = new QVBoxLayout;

//            mainWindow->setWindowTitle("同步");
//            // 创建GitSync类对象并添加到主窗口中
//            gitSync = new GitSync;
//            mainLayout->addWidget(gitSync);

//            mainWindow->setLayout(mainLayout);
//            mainWindow->show();
//            moveToCenter(mainWindow); //把窗口移动到屏幕中间


//        });


        connect(db3,&DButtonBoxButton::clicked,this,[=](){
//            QString path=QFileDialog::getExistingDirectory(this,tr("选择创建文件路径"),"../");
//            if(path!=nullptr)
//            {

//                QString sName = QInputDialog::getText(this,
//                                             "QInputdialog_Name",
//                                             "请输入笔记名称",
//                                             QLineEdit::Normal,
//                                             "note"
//                                             );
//                if(sName!=nullptr)
//                {
//                    w->creatFile(sName+".md");
//                    w->fileSave();
//                    w->changePath(path);
//                    w->openFile(path+"/"+sName+".md");
//                }
//                else {
//                    QMessageBox::warning(this,tr("创建失败"),tr("未输入笔记名称"));
//                }
//            }
//            else
//            {
//                QMessageBox::warning(this,tr("创建失败"),tr("未选择笔记存储路径"));
//            }
//        });
//        connect(db2,&DButtonBoxButton::clicked,this,[=](){
//            QString path=QFileDialog::getExistingDirectory(this,tr("选择创建文件路径"),"../");
//            if(path!=nullptr)
//            {
//                QString sName = QInputDialog::getText(this,
//                                                         "QInputdialog_Name",
//                                                         "请输入笔记本名称",
//                                                         QLineEdit::Normal,
//                                                         "notebook"
//                                                         );
//                if(sName!=nullptr)
//                {
//                    w->creatDir(path,sName);
//                    w->changePath(path+"/"+sName);
//                }
//                else
//                {
//                    QMessageBox::warning(this,tr("创建失败"),tr("未输入笔记本名称"));
//                }
//            }
//            else
//            {
//                QMessageBox::warning(this,tr("创建失败"),tr("未选择笔记本存储路径"));
//            }



                QString sName = QInputDialog::getText(this,
                                             "QInputdialog_Name",
                                             "请输入笔记名称",
                                             QLineEdit::Normal,
                                             "note"
                                             );
                if(sName!=nullptr)
                {
                    w->creatFile(sName+".md");
                    w->fileSave();
                    //w->changePath(path);
                    //w->openFile(path+"/"+sName+".md");
                }
                else {
//                    QMessageBox::warning(this,tr("创建失败"),tr("未输入笔记名称"));
                }


        });
        connect(db2,&DButtonBoxButton::clicked,this,[=](){

                QString sName = QInputDialog::getText(this,
                                                         "QInputdialog_Name",
                                                         "请输入笔记本名称",
                                                         QLineEdit::Normal,
                                                         "notebook"
                                                         );
                if(sName!=nullptr)
                {
                    w->creatDir(sName);
//                    w->changePath(path+"/"+sName);
                }
                else
                {
//                    QMessageBox::warning(this,tr("创建失败"),tr("未输入笔记本名称"));
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
                                                         "导出文件",
                                                         "请输入文件名",
                                                         QLineEdit::Normal,
                                                         "filename"
                                                         );
                if(sName!=nullptr)
                {
                    fileName=path+"/"+sName;

                    // 创建一个询问框，并添加三个自定义按钮
                    QMessageBox msgBox;
                    msgBox.setText("请选择导出文件格式");
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
                        //QMessageBox::information(nullptr, "Answer", "pdf&word");
                        w->coutPdf(fileName);
                        w->coutWord(fileName);
                    }
                }
                else
                {
//                    QMessageBox::warning(this,tr("导出失败"),tr("未输入导出文件名"));
                }
            }
            else
            {
//                 QMessageBox::warning(this,tr("导出失败"),tr("未选择导出文件存储路径"));
            }
        });

        connect(db7,&DButtonBoxButton::clicked,this,[=](){
            //w->fileSave();
w->fileSave_2();

        });

        connect(db8,&DButtonBoxButton::clicked,this,[=](){
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
    if (w->isModified())
    {
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
    if (w->isModified())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to exit anyway?"));
        if (button != QMessageBox::Yes)
        {
            e->ignore();
        }
    }
}
