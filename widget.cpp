#include "widget.h"
#include "ui_widget.h"
#include <QWebChannel>
#include "previewpage.h"
#include <QSplitter>
#include <iostream>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

Widget::Widget(QWidget *parent) :
    DWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    lab=new DLabel(this);
//    lab->setText("Hello DTK");


//QSplitter *splitter=new QSplitter(ui->horizontalWidget);
//splitter->addWidget(ui->editor);
//splitter->addWidget(ui->preview);
//splitter->setMinimumSize(this->height()*2,this->width());

    ui->editor->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    ui->preview->setContextMenuPolicy(Qt::NoContextMenu);
//编辑界面和预览界面的切换
    ui->preview->hide();

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


    //信号与槽的连接
    connect(ui->view, &QToolButton::clicked, this,&Widget::on_view_clicked);
    connect(ui->edite,&QToolButton::clicked, this,&Widget::on_edite_clicked);

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
    QFileDialog dialog(this, tr("Save MarkDown File"));
    dialog.setMimeTypeFilters({"text/markdown"});
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("md");
    if (dialog.exec() != QDialog::Accepted)
        return;

    m_filePath = dialog.selectedFiles().constFirst();
    fileSave();
}

void Widget::on_view_clicked()
{
    ui->editor->hide();
    ui->preview->show();
}

void Widget::on_edite_clicked()
{
    ui->preview->hide();
    ui->editor->show();
}
