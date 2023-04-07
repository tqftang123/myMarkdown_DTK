/****************************************************************************
**
** 中央窗口界面类
**
****************************************************************************/

#ifndef WIDGET_H
#define WIDGET_H


#include <DWidget>
#include <DLabel>
#include <DDialog>
#include <DButtonBox>
#include <DTitlebar>
#include <QDirModel>

#include "document.h"


//加载dtk的名字空间
DWIDGET_USE_NAMESPACE

namespace Ui {
class Widget;
}

class Widget : public DWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

//打开文件的具体实现函数
    void openFile(const QString &path);

    //当前文件是否已修改
    bool isModified() const;
    void fileNew();
    void fileSave();
    void fileSaveAs();

    //更新文件目录结构
    void treeViewChage();
    //创建文件
    void creatFile(const QString path,const QString suffix);
    void creatDir(const QString path,const QString name);

    //编辑、预览的切换
    void view();
    void edite();
    //导出为PDF格式
    void coutPdf(QString fileName);

private slots:


    void on_bold_clicked();

    void on_treeView_clicked(const QModelIndex &index);

    void on_italic_clicked();

    void on_strickout_clicked();

    void on_list_ordered_clicked();

    void on_list_unordered_clicked();

    void on_table_clicked();

private:
    Ui::Widget *ui;
//    DLabel *lab;
    QString m_filePath;
    Document m_content;

    QDirModel *model;

};

#endif // WIDGET_H
