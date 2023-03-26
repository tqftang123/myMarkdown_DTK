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

private slots:
    void on_view_clicked();
    void on_edite_clicked();

private:
    Ui::Widget *ui;
//    DLabel *lab;
    QString m_filePath;
    Document m_content;

};

#endif // WIDGET_H
