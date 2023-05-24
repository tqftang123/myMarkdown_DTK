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
#include <QListView>
#include <QStringListModel>

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
    void fileSave_2();
    void fileSaveAs();

    //更新文件目录结构
    void treeViewChage();
    //创建文件
    //void creatFile(const QString path,const QString suffix);
    void creatFile(const QString suffix);
    void creatDir(const QString name);

    //编辑、预览的切换
    void view();
    void edite();
    //导出为PDF格式
    void coutPdf(QString fileName);
    void changPrive();
    void coutWord(QString fileName);

    //导入word文件
    void cinWord(QString filePath);

    //切换笔记本位置（左边那个显示的文件夹位置）
    void changePath(QString path);

    //更新打开文件的list列表
    void updataList();



private slots:

    void on_bold_clicked();

    void on_treeView_clicked(const QModelIndex &index);

    void on_italic_clicked();

    void on_strickout_clicked();

    void on_list_ordered_clicked();

    void on_list_unordered_clicked();

    void on_table_clicked();
    //listview
    void slotClicked(const QModelIndex &index);

    //关闭当前打开的文件
    void on_close_clicked();

    //目录按钮被点击后
    void on_content_clicked();
    //专注模式
    void on_concentrated_clicked();
    //正常模式
    void on_normal_clicked();

    void on_image_clicked();

    void on_heading_clicked();

    void on_code_clicked();

    void on_link_clicked();

    void on_dividing_line_clicked();

    void on_quote_clicked();

    //模板库
    void on_demo_clicked();

    void on_log_clicked();

private:
    Ui::Widget *ui;
    QString m_filePath;
    Document m_content;
    QDirModel *model;
    QStringList list;                                //创建数据显示列表
    QStringList listUrl;
    QStringListModel *listmodel;
    int current=0;
    QString m_fileName;
//当前目录的文件夹路径
QString path;

public:
QString url2;

};

class MyDirModel :public QDirModel{
public:
    //目录显示为文件名
    QVariant headerData(int section,Qt::Orientation orientation,int role = Qt::DisplayRole) const{
        if(role == Qt::DisplayRole && orientation == Qt::Horizontal){
            if(section == 0){
                return QString("文件名");
            }
        }
        return QDirModel::headerData(section,orientation,role);
    }

    //使目录结构只显示文件名那一列
    int columnCount(const QModelIndex &parent = QModelIndex()) const{
        Q_UNUSED(parent);
        return 1;
    }
    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const{
        //处理图标
        if(role == Qt::DecorationRole){
            if(index.column() == 0){
                QFileInfo fileInfo = this->fileInfo(index);
                if(fileInfo.isFile()){
                    QFileIconProvider iconProvider;
                    return iconProvider.icon(QFileIconProvider::File);
                }
                else if (fileInfo.isDir()) {
                    QFileIconProvider iconProvider;
                    return iconProvider.icon(QFileIconProvider::Folder);
                }
            }
        }

        if(role != Qt::DisplayRole && role != Qt::ToolTipRole){
            return QVariant();
        }
        if(index.column() != 0){
            return QVariant();
        }
        return QDirModel::data(index,Qt::DisplayRole);
    }
};

#endif // WIDGET_H
