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

Widget::Widget(QWidget *parent) :
    DWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    lab=new DLabel(this);
//    lab->setText("Hello DTK");

//QSplitter可调节两个窗口的大小
//QSplitter *splitter=new QSplitter(ui->horizontalWidget);
//splitter->addWidget(ui->editor);
//splitter->addWidget(ui->preview);
//splitter->setMinimumSize(this->height()*2,this->width());

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

    QFile defaultTextFile(":/resources/default.md");
    defaultTextFile.open(QIODevice::ReadOnly);
    ui->editor->setPlainText(defaultTextFile.readAll());



    ///////////保存为PDF
   // QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Portrait, QMarginsF());
    //ui->preview->page()->printToPdf(QStringLiteral("/home/rootroot/t/3p出厂检验报告.pdf"),layout);

//ui->preview->page()->printToPdf(QStringLiteral("/home/rootroot/t/12.pdf"));



//    //信号与槽的连接(这个是原来的编辑与预览按钮的connect，现在移动的按钮的位置，此代码无用了
//    connect(ui->view, &QToolButton::clicked, this,&Widget::on_view_clicked);
//    connect(ui->edite,&QToolButton::clicked, this,&Widget::on_edite_clicked);

    //不用connect，哦，ui界面转到槽函数的
    //connect(ui->bold,&QToolButton::clicked, this,&Widget::on_bold_clicked);


//展示目录结构
//方法1：
        //QDirModel *model = new QDirModel();
    model = new QDirModel();
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index("/home/rootroot/t"));

    m_filePath="/home/rootroot/t";

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
}

Widget::~Widget()
{
    delete ui;
}




void Widget::openFile(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, windowTitle(),
                             tr("Could not open file %1: %2").arg(
                                 QDir::toNativeSeparators(path), f.errorString()));
        return;
    }
    m_filePath = path;
    ui->editor->setPlainText(f.readAll());
    //statusBar()->showMessage(tr("Opened %1").arg(QDir::toNativeSeparators(path)));

    //更新打开的文件名
    ui->label->setText(path);
}


bool Widget::isModified() const
{
    return ui->editor->document()->isModified();
}




void Widget::fileNew()
{
    if (isModified()) {
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
    //保存文件之后，更新目录结构
    this->treeViewChage();

    if (m_filePath.isEmpty()) {
        fileSaveAs();
        return;
    }

    QFile f(m_filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))  {
        QMessageBox::warning(this, windowTitle(),
                             tr("Could not write to file %1: %2").arg(
                                 QDir::toNativeSeparators(m_filePath), f.errorString()));


        return;
    }
    QTextStream str(&f);
    str << ui->editor->toPlainText();

    ui->editor->document()->setModified(false);

    //statusBar()->showMessage(tr("Wrote %1").arg(QDir::toNativeSeparators(m_filePath)));




}

void Widget::fileSaveAs()
{
    //保存文件之后，更新目录结构
    this->treeViewChage();

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
    //啊，还没有实现此功能
    //ui->treeView->setRootIndex(model->index("/home/rootroot/t"));

    //哈哈，原来这样就可以了哎
    model = new QDirModel();
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index("/home/rootroot/t"));

}


void Widget::creatFile(const QString path,const QString suffix)
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

}

void Widget::creatDir(const QString path,const QString name)
{

       //要创建的文件夹名称

    QDir dir(path);
    if(!dir.exists(name))    //如果文件夹不存在
    {
        dir.mkdir(name);    //创建文件夹（名为name）

        qDebug()<<QString("文件夹%1创建成功！").arg(name);
    }
    else
    {
        qDebug()<<QString("文件夹%1已存在！").arg(name);
    }

}

void Widget::view()
{
    ui->editor->hide();
    ui->preview->show();

//    ui->bold->setIcon(QIcon(":/images/icon-bold.svg"));
//    ui->bold->setEnabled(false);
    ui->bold->setIcon(QIcon(":/images/icon-bold.svg"));
    ui->image->setIcon(QIcon(":/images/icon-file-media.svg"));
    ui->italic->setIcon(QIcon(":/images/icon-italic.svg"));
    ui->link->setIcon(QIcon(":/images/icon-link.svg"));
    ui->list_ordered->setIcon(QIcon(":/images/icon-list-ordered.svg"));
    ui->list_unordered->setIcon(QIcon(":/images/icon-list-unordered.svg"));
    ui->quote->setIcon(QIcon(":/images/icon-quote.svg"));
    ui->strickout->setIcon(QIcon(":/images/icon-strickout.svg"));
    ui->table->setIcon(QIcon(":/images/icon-table.svg"));

    ui->bold->setEnabled(false);
    ui->image->setEnabled(false);
    ui->italic->setEnabled(false);
    ui->link->setEnabled(false);
    ui->list_ordered->setEnabled(false);
    ui->list_unordered->setEnabled(false);
    ui->quote->setEnabled(false);
    ui->strickout->setEnabled(false);
    ui->table->setEnabled(false);


    ///////////保存为PDF
    //ui->preview->page()->printToPdf(QStringLiteral("/home/rootroot/t/12.pdf"));

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

    ui->bold->setEnabled(true);
    ui->image->setEnabled(true);
    ui->italic->setEnabled(true);
    ui->link->setEnabled(true);
    ui->list_ordered->setEnabled(true);
    ui->list_unordered->setEnabled(true);
    ui->quote->setEnabled(true);
    ui->strickout->setEnabled(true);
    ui->table->setEnabled(true);


}



 void Widget::coutPdf(QString fileName )
 {
     ///////////保存为PDF
     //QString fileName="/home/rootroot/t/12.pdf";
     //ui->preview->page()->printToPdf(QStringLiteral("/home/rootroot/t/12.pdf"));
     ui->preview->page()->printToPdf(fileName);
 }

//目录被点击之后
void Widget::on_treeView_clicked(const QModelIndex &index)
{
    //m_filePath=model->filePath(index);
    QFileInfo fileInfo = QFileInfo(model->filePath(index));
    //文件名
    QString fileName = fileInfo.fileName();
    //文件后缀
    QString fileSuffix = fileInfo.suffix();
    //绝对路径
    //QString filePath = fileInfo.absolutePath();

    //只能打开md文件后缀名的文件
    if(fileSuffix=="md")
    {
        this->openFile(model->filePath(index));

        //更新打开的文件名
        ui->label->setText(fileName);
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

    //光标向左移动格，似上面添加的文本而定数字，目的是把光标移动到内部
//    for(int i=0;i<11;i++)
//    {
//        ui->editor->moveCursor(QTextCursor::Left,QTextCursor::MoveAnchor);
//    }
}
