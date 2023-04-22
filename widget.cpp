#include "widget.h"
#include "ui_widget.h"
#include <QWebChannel>
#include "previewpage.h"
#include <QSplitter>
#include <iostream>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDirModel>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QPageLayout>
#include <QProcess>
#include <QTimer>

Widget::Widget(QWidget *parent) :
    DWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

//如果我需要编辑和预览一左一右到话
//QSplitter可调节两个窗口的大小
//QSplitter *splitter=new QSplitter(ui->horizontalWidget);
//splitter->addWidget(ui->editor);
//splitter->addWidget(ui->preview);
//splitter->setMinimumSize(this->height()*3,this->width());

    ui->editor->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    ui->preview->setContextMenuPolicy(Qt::NoContextMenu);
    //编辑界面和预览界面的切换
    ui->preview->hide();

    //文本和html的链接，预览功能的实现
    PreviewPage *page = new PreviewPage(this);
    ui->preview->setPage(page);

    connect(ui->editor, &QPlainTextEdit::textChanged,
            [this]() { m_content.setText(ui->editor->toPlainText()); });

    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &m_content);
    page->setWebChannel(channel);
    ui->preview->setUrl(QUrl("qrc:/resources/index.html"));
    //ui->preview->setUrl(QUrl("qrc:/resources/index-1.html"));

    QFile defaultTextFile(":/resources/default.md");
    defaultTextFile.open(QIODevice::ReadOnly);
    ui->editor->setPlainText(defaultTextFile.readAll());


    //展示目录结构
    //方法1：
    //QDirModel *model = new QDirModel();
    model = new QDirModel();
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index("./"));
    m_filePath="./new.md";
    //方法2：
//    QFileSystemModel *model = new QFileSystemModel;
//    model->setRootPath(QDir::currentPath()); //设置根目录
//    //model->setRootPath(m_filePath);
//    ui->treeView->setModel(model);


    //光标是否在屏幕上居中
    ui->editor->setCenterOnScroll(true);

    //显示打开的文件路径
    ui->label->setText(m_filePath);

    //设置图标
    ui->bold->setIcon(QIcon(":/images/icon-bold-down.svg"));
    ui->image->setIcon(QIcon(":/images/icon-file-media-down.svg"));
    ui->italic->setIcon(QIcon(":/images/icon-italic-down.svg"));
    ui->link->setIcon(QIcon(":/images/icon-link-down.svg"));
    ui->list_ordered->setIcon(QIcon(":/images/icon-list-ordered-down.svg"));
    ui->list_unordered->setIcon(QIcon(":/images/icon-list-unordered-down.svg"));
    ui->quote->setIcon(QIcon(":/images/icon-quote-down.svg"));
    ui->strickout->setIcon(QIcon(":/images/icon-strickout-down.svg"));
    ui->table->setIcon(QIcon(":/images/icon-table-down.svg"));
    ui->code->setIcon(QIcon(":images/icon-code-down.svg"));
    ui->heading->setIcon(QIcon(":images/icon-hide-down.svg"));
    ui->dividing_line->setIcon(QIcon(":images/icon-fenge-down.svg"));

    //控件的toolTip 设置提示悬浮
    ui->bold->setToolTip("加粗");
    ui->image->setToolTip("图片");
    ui->italic->setToolTip("斜体");
    ui->link->setToolTip("链接");
    ui->list_ordered->setToolTip("有序列表");
    ui->list_unordered->setToolTip("无序列表");
    ui->quote->setToolTip("引用");
    ui->strickout->setToolTip("删除线");
    ui->table->setToolTip("表格");
    ui->code->setToolTip("代码");
    ui->heading->setToolTip("标题");
    ui->dividing_line->setToolTip("分割线");

    //m_filePath="/home/rootroot/t/newFile.md";


    //使用数据列表创建数据显示模型
    //QStringListModel *listmodel = new QStringListModel(list);
    listmodel = new QStringListModel(list);
    ui->listView->setModel(listmodel);                   //设置模型到listview上

    ui->listView->setMovement(QListView::Free);          //设置数据可以自由拖动
    ui->listView->setSpacing(2);                         //设置数据的间距
    ui->listView->setFlow(QListView::LeftToRight);

    connect(ui->listView, SIGNAL(clicked(const QModelIndex)),
                this, SLOT(slotClicked(const QModelIndex)));

    //快捷键的设置,加粗的快捷键
    ui->bold->setShortcut(tr("ctrl+b"));
    ui->italic->setShortcut(tr("ctrl+i"));
    ui->heading->setShortcut(tr("ctrl+h"));
    ui->image->setShortcut(tr("ctrl+g"));
    ui->link->setShortcut(tr("ctrl+l"));
    ui->list_ordered->setShortcut(tr("ctrl+o"));
    ui->list_unordered->setShortcut(tr("ctrl+u"));
    ui->quote->setShortcut(tr("ctrl+q"));
    ui->strickout->setShortcut(tr("ctrl+d"));
    ui->table->setShortcut(tr("ctrl+t"));
    ui->code->setShortcut(tr("ctrl+c"));

    //默认打开程序时为正常模式
    ui->concentrated->setEnabled(true);
    ui->normal->setEnabled(false);


ui->close->setIcon(QIcon(":/images/icon-close.svg"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::openFile(const QString &path)
{
    m_filePath = path;
    //更新打开的文件路径
    ui->label->setText(path);

    QFileInfo fileInfo = QFileInfo(path);
    //文件名
    QString fileName = fileInfo.fileName();
    m_fileName=fileInfo.fileName();
    //设置同一个文件只能打开一次
    int flage=0;
    int i=0;
    for(i=0;i<list.size();i++)
    {
        if(list[i]==fileName&&listUrl[i]==m_filePath)
        {
            flage=1;
            QFile f(listUrl[i]);
            if (!f.open(QIODevice::ReadOnly))
            {
                QMessageBox::warning(this, windowTitle(),
                             tr("Could not open file %1: %2").arg(
                                 QDir::toNativeSeparators(path), f.errorString()));
                return;
            }
            ui->editor->setPlainText(f.readAll());
            //记录当前打开文件是第几个
            current=i;
        }
    }
    if(flage==0)
    {
        list.append(fileName);
        listUrl.append(m_filePath);
        //////记录当前打开文件是第几个
        current=i;
        //更新打开文件的list列表
        this->updataList();
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this, windowTitle(),
                         tr("Could not open file %1: %2").arg(
                             QDir::toNativeSeparators(path), f.errorString()));
            return;
        }
        ui->editor->setPlainText(f.readAll());
    }
}

bool Widget::isModified() const
{
    return ui->editor->document()->isModified();
}

void Widget::fileNew()
{
    if (isModified())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to create a new document anyway?"));
        if (button != QMessageBox::Yes)
            return;
    }
    m_filePath.clear();
    ui->editor->setPlainText(tr("## New document"));
    ui->editor->document()->setModified(false);
}

void Widget::fileSave()
{
    //保存文件之后，更新目录
    //this->changePath(path);


//    if (m_filePath.isEmpty()) {
//        fileSaveAs();
//        return;
//    }

    QFile f(m_filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
//        QMessageBox::warning(this, windowTitle(),
//                             tr("Could not write to file %1: %2").arg(
//                                 QDir::toNativeSeparators(m_filePath), f.errorString()));


        return;
    }
    QTextStream str(&f);
    str << ui->editor->toPlainText();
    ui->editor->document()->setModified(false);
}



void Widget::fileSave_2()
{
    //保存文件之后，更新目录
    //this->changePath(path);


    if (m_filePath.isEmpty()) {
        fileSaveAs();
        return;
    }

    QFile f(m_filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
//        QMessageBox::warning(this, windowTitle(),
//                             tr("Could not write to file %1: %2").arg(
//                                 QDir::toNativeSeparators(m_filePath), f.errorString()));


        return;
    }
    QTextStream str(&f);
    str << ui->editor->toPlainText();
    ui->editor->document()->setModified(false);

   QMessageBox::information(this,"提示","保存成功");


}


void Widget::fileSaveAs()
{
    //保存文件之后，更新目录
    this->changePath(path);

    QFileDialog dialog(this, tr("Save MarkDown File"));
    dialog.setMimeTypeFilters({"text/markdown"});
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("md");
    if (dialog.exec() != QDialog::Accepted)
        return;

    m_filePath = dialog.selectedFiles().constFirst();
    fileSave();
}


void Widget::treeViewChage()
{
    model = new QDirModel();
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(path));
}


void Widget::creatFile(const QString suffix)
{

    QString fileName=path+"/"+suffix;
    QFile file(fileName);

    if(file.exists())
    {
        openFile(fileName);
    }
    else
    {
        file.open(QFile::WriteOnly|QFile::Text|QIODevice::Append);
        openFile(fileName);
    }
    changePath(path);
    openFile(path+"/"+suffix);
}

void Widget::creatDir(const QString name)
{
    //要创建的文件夹名称
    QDir dir(this->path);
    if(!dir.exists(name))    //如果文件夹不存在
    {
        dir.mkdir(name);    //创建文件夹（名为name）

        //QMessageBox::warning(this,tr("创建成功"),tr("文件夹创建成功！"));
        qDebug()<<QString("文件夹%1创建成功！").arg(name);
    }
    else
    {
        QMessageBox::warning(this,tr("创建失败"),tr("文件夹已存在！"));
        qDebug()<<QString("文件夹%1已存在！").arg(name);

    }
    changePath(this->path);
}

void Widget::view()
{
    ui->preview->setUrl(QUrl("qrc:/resources/index.html"));

    ui->editor->hide();
    ui->preview->show();

    ui->bold->setIcon(QIcon(":/images/icon-bold.svg"));
    ui->image->setIcon(QIcon(":/images/icon-file-media.svg"));
    ui->italic->setIcon(QIcon(":/images/icon-italic.svg"));
    ui->link->setIcon(QIcon(":/images/icon-link.svg"));
    ui->list_ordered->setIcon(QIcon(":/images/icon-list-ordered.svg"));
    ui->list_unordered->setIcon(QIcon(":/images/icon-list-unordered.svg"));
    ui->quote->setIcon(QIcon(":/images/icon-quote.svg"));
    ui->strickout->setIcon(QIcon(":/images/icon-strickout.svg"));
    ui->table->setIcon(QIcon(":/images/icon-table.svg"));
    ui->code->setIcon(QIcon(":/images/icon-code.svg"));
    ui->heading->setIcon(QIcon(":images/icon-hide.svg"));
    ui->dividing_line->setIcon(QIcon(":images/icon-fenge.svg"));

    ui->bold->setEnabled(false);
    ui->image->setEnabled(false);
    ui->italic->setEnabled(false);
    ui->link->setEnabled(false);
    ui->list_ordered->setEnabled(false);
    ui->list_unordered->setEnabled(false);
    ui->quote->setEnabled(false);
    ui->strickout->setEnabled(false);
    ui->table->setEnabled(false);
    ui->code->setEnabled(false);
    ui->heading->setEnabled(false);
    ui->dividing_line->setEnabled(false);
}

void Widget::edite()
{
    ui->preview->hide();
    ui->editor->show();

    ui->bold->setIcon(QIcon(":/images/icon-bold-down.svg"));
    ui->image->setIcon(QIcon(":/images/icon-file-media-down.svg"));
    ui->italic->setIcon(QIcon(":/images/icon-italic-down.svg"));
    ui->link->setIcon(QIcon(":/images/icon-link-down.svg"));
    ui->list_ordered->setIcon(QIcon(":/images/icon-list-ordered-down.svg"));
    ui->list_unordered->setIcon(QIcon(":/images/icon-list-unordered-down.svg"));
    ui->quote->setIcon(QIcon(":/images/icon-quote-down.svg"));
    ui->strickout->setIcon(QIcon(":/images/icon-strickout-down.svg"));
    ui->table->setIcon(QIcon(":/images/icon-table-down.svg"));
    ui->code->setIcon(QIcon(":/images/icon-code-down.svg"));
    ui->heading->setIcon(QIcon(":images/icon-hide-down.svg"));
    ui->dividing_line->setIcon(QIcon(":images/icon-fenge-down.svg"));

    ui->bold->setEnabled(true);
    ui->image->setEnabled(true);
    ui->italic->setEnabled(true);
    ui->link->setEnabled(true);
    ui->list_ordered->setEnabled(true);
    ui->list_unordered->setEnabled(true);
    ui->quote->setEnabled(true);
    ui->strickout->setEnabled(true);
    ui->table->setEnabled(true);
    ui->code->setEnabled(true);
    ui->heading->setEnabled(true);
    ui->dividing_line->setEnabled(true);

    this->changPrive();
}

 void Widget::coutPdf(QString fileName )
 {
     ///////////保存为PDF，ui->preview->page()->printToPdf(fileName);
     //QString fileName="/home/rootroot/t/12.pdf";
     //ui->preview->page()->printToPdf(QStringLiteral("/home/rootroot/t/12.pdf"));

     this->edite();
     //改变文件结构那个，有点问题，只有点击编辑之后再点导出就是没有文件结构的，在预览界面进行导出就有文件结构
     //this->changPrive();
     ui->preview->page()->printToPdf(fileName+".pdf");
 }

 void Widget::changPrive()
 {
     ui->preview->setUrl(QUrl("qrc:/resources/index-1.html"));
     emit(ui->editor->textChanged());
     ui->preview->update();
 }

 //导出为Word格式的文件
 void Widget::coutWord(QString fileName)
 {
     QString docxFile=fileName+".docx";
     QString mdFile=m_filePath;

     QString program = "pandoc";
     QStringList arguments;
     arguments << "-f" << "markdown" << "-t" << "docx" << "-o" << docxFile << mdFile;

     QProcess process;
     process.start(program, arguments);
     process.waitForFinished();

     if (process.exitCode() != 0)
     {
         qWarning() << "Error converting docx to markdown:" << process.errorString();
     }
 }


 void Widget::cinWord(QString filePath)
 {
     QString mdFile="./word.md";
     QString program = "pandoc";
     QStringList arguments;

     //docx导出为md文件
     arguments << "-f" << "docx" << "-t" << "markdown" << "-o" << mdFile << filePath;

     QProcess process;
     process.start(program, arguments);
     process.waitForFinished();

     if (process.exitCode() != 0)
     {
         //qWarning() << "Error converting docx to markdown:" << process.errorString();
     }

     this->openFile("./word.md");
 }


 void Widget::changePath(QString path)
 {
      model = new QDirModel();
      ui->treeView->setModel(model);
      ui->treeView->setRootIndex(model->index(path));
      this->path=path;
 }


 //更新打开文件的list列表
 void Widget::updataList()
 {
     //更新数据
     listmodel = new QStringListModel(list);
     ui->listView->setModel(listmodel);                   //设置模型到listview上

     ui->listView->setMovement(QListView::Free);          //设置数据可以自由拖动
     ui->listView->setSpacing(2);                         //设置数据的间距
     ui->listView->setFlow(QListView::LeftToRight);
 }






//目录被点击之后
void Widget::on_treeView_clicked(const QModelIndex &index)
{
    m_filePath=model->filePath(index);

    QFileInfo fileInfo = QFileInfo(model->filePath(index));
    //文件名
    QString fileName = fileInfo.fileName();
    m_fileName=fileInfo.fileName();
    //文件后缀
    QString fileSuffix = fileInfo.suffix();
    //绝对路径
    //QString filePath = fileInfo.absolutePath();

    //只能打开md文件后缀名的文件
    if(fileSuffix=="md")
    {
        this->openFile(model->filePath(index));
    }
    //当点击的不是md文件时，提示当前程序只允许打开md文件
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("当前程序只允许打开md文件!!!"));
    }
}

void Widget::slotClicked(const QModelIndex &index)
{
    for(int i=0;i<list.size();i++)
    {
        if(list[i]==index.data())
        {
            openFile(listUrl[i]);
            //记录当前打开文件是第几个
            current=i;
            break;
        }
    }
}

//加粗按钮被点击后
void Widget::on_bold_clicked()
{
    //啊，这样只能在文件末尾添加，应该在光标位置添加才对
    //appendPlainText()在文本末尾添加一个新段落和文本
    //ui->editor->appendPlainText(tr("**bolds**"));

    //哈哈，可以了，insertPlainText（）在当前光标位置插入文本
    ui->editor->insertPlainText(tr("**bolds**"));

    //光标向左移动7格，似上面添加的文本而定数字，目的是把光标移动到内部
    for(int i=0;i<7;i++)
    {
        ui->editor->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
    }

    //效果同上面代码，在当前光标位置插入文本
    //ui->editor->textCursor().insertText(tr("**bolds**"));
}

void Widget::on_italic_clicked()
{
    ui->editor->insertPlainText(tr("*italic*"));

    //光标向左移动7格，似上面添加的文本而定数字，目的是把光标移动到内部
    for(int i=0;i<7;i++)
    {
        ui->editor->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
    }
}

void Widget::on_strickout_clicked()
{
    ui->editor->insertPlainText(tr("~~stickout~~"));

    //光标向左移动格，似上面添加的文本而定数字，目的是把光标移动到内部
    for(int i=0;i<10;i++)
    {
        ui->editor->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
    }
}

void Widget::on_list_ordered_clicked()
{
    ui->editor->insertPlainText(tr("1. 第一项\n2. 第二项\n3. 第三项"));

    //光标向左移动格，似上面添加的文本而定数字，目的是把光标移动到内部
    for(int i=0;i<14;i++)
    {
        ui->editor->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
    }
}

void Widget::on_list_unordered_clicked()
{
    ui->editor->insertPlainText(tr("* 第一项\n* 第二项\n* 第三项"));

    //光标向左移动格，似上面添加的文本而定数字，目的是把光标移动到内部
    for(int i=0;i<12;i++)
    {
        ui->editor->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
    }
}

void Widget::on_table_clicked()
{
    ui->editor->insertPlainText(tr("|  表头   | 表头  | \n|  ----  | ----  |\n| 单元格  | 单元格 |\n| 单元格  | 单元格 |"));
}



void Widget::on_dome_clicked()
{
    this->changePath("/home/rootroot/t/untitled/dome/");
    this->openFile("/home/rootroot/t/untitled/dome//dome-day.md");
}

void Widget::on_close_clicked()
{
    //已经修改的文件在关闭时自动保存
//    if(isModified())
//    {
//        this->fileSave_2();
//    }

/////
    list.removeAt(current);
    listUrl.removeAt(current);
    //更新打开文件的list列表
      this->updataList();
    //打开前一个文件
    if(current>0)
    {
        this->openFile(listUrl[current-1]);
        m_filePath=listUrl[current-1];

    }
    //当关闭的是第一个文件时，如果list中还有文件，则打开第0个（因为此时current=0）
    else if(list.length()>0)
    {
        this->openFile(listUrl[current]);
        m_filePath=listUrl[current];
    }
    //list中没有文件时，新建一个文件
    else
    {
        //this->fileNew();
        //m_filePath="";
        ui->editor->setPlainText(tr(""));
        //ui->editor->document()->setModified(false);
        ui->label->setText("");
    }
}



void Widget::on_content_clicked()
{
    QString path=QFileDialog::getExistingDirectory(this,tr("选择打开的目录路径"),"../");
    this->changePath(path);
}

//专注模式，全屏模式
void Widget::on_concentrated_clicked()
{
    ui->treeView->hide();
    ui->content->hide();
    ui->log->hide();
    ui->dome->hide();
    ui->concentrated->setEnabled(false);
    ui->normal->setEnabled(true);

ui->listView->hide();
ui->bold->hide();
ui->code->hide();
ui->dividing_line->hide();
ui->heading->hide();
ui->image->hide();
ui->italic->hide();
ui->link->hide();
ui->list_ordered->hide();
ui->list_unordered->hide();
ui->quote->hide();
ui->strickout->hide();
ui->table->hide();

}
//正常模式
void Widget::on_normal_clicked()
{
    ui->treeView->show();
    ui->content->show();
    ui->log->show();
    ui->dome->show();

    ui->listView->show();
    ui->bold->show();
    ui->code->show();
    ui->dividing_line->show();
    ui->heading->show();
    ui->image->show();
    ui->italic->show();
    ui->link->show();
    ui->list_ordered->show();
    ui->list_unordered->show();
    ui->quote->show();
    ui->strickout->show();
    ui->table->show();

    ui->concentrated->setEnabled(true);
    ui->normal->setEnabled(false);

}

void Widget::on_image_clicked()
{
    //QString path=QDir::currentPath();

    // 创建文件选择框
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    // 复制图片到指定文件夹中
    QString targetDir = path + "/mk_images"; // 指定目标文件夹
    //如果没有文件夹则创建该文件夹
    QDir targetFolder(targetDir);
    if(!targetFolder.exists()){
        targetFolder.mkpath(".");
    }
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeStr = dateTime.toString("yyyy_MM_dd_HH_mm_ss");
    QString targetFileName = targetDir + "/" + "Markdown_"+dateTimeStr+"_"+QFileInfo(fileName).fileName(); // 目标文件名
    QFile::copy(fileName, targetFileName);

//    QString gitDir = path +"/"+ m_repoName;
//    QMessageBox::information(this,tr("123"),m_repoName);
//    QDir gitFolder(gitDir);
//    QString gitFileName = gitDir+ "/" + "Markdown_"+dateTimeStr+"_"+QFileInfo(fileName).fileName();
//    if(gitFolder.exists()){
//        QFile::copy(fileName, gitFileName);
//    }

    //等待用户选择确认按钮
    if(!fileName.isEmpty()){
        ui->editor->insertPlainText("![](file://" + targetFileName + ")");
    }
}

void Widget::on_heading_clicked()
{
    ui->editor->insertPlainText(tr("# heading"));
    for(int i=0;i<7;i++)
    {
        ui->editor->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
    }
}

void Widget::on_code_clicked()
{
    ui->editor->insertPlainText(tr("`world`"));
    for(int i=0;i<6;i++)
    {
        ui->editor->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
    }
}



void Widget::on_link_clicked()
{
    ui->editor->insertPlainText(tr("[新浪新闻](https://news.sina.com.cn/)"));
}

void Widget::on_dividing_line_clicked()
{
    ui->editor->insertPlainText(tr("----------"));
}

void Widget::on_quote_clicked()
{
    ui->editor->insertPlainText(">");
}
