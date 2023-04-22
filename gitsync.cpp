#include "gitsync.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QInputDialog>

GitSync::GitSync(QWidget *parent) :
    QWidget(parent),
//    m_usernameEdit(new QLineEdit),
//    m_passwordEdit(new QLineEdit),
//    m_repoEdit(new QLineEdit),
//    m_loginBtn(new QPushButton(tr("Login"))),
//    m_statusLabel(new QLabel),
//    m_syncToLocalBox(new QCheckBox(tr("Sync to local"))),
//    m_syncToRemoteBox(new QCheckBox(tr("Sync to remote"))),
//    m_syncBtn(new QPushButton(tr("Sync"))),
//    m_progressBar(new QProgressBar),
    m_username(""),
    m_password(""),
    m_repoUrl("")
{
//    // 设置密码框显示模式为密码模式
//    m_passwordEdit->setEchoMode(QLineEdit::Password);

//    // 使用垂直布局和水平布局组合构建界面
//    QVBoxLayout *mainLayout = new QVBoxLayout;
//    QHBoxLayout *loginLayout = new QHBoxLayout;
//    QHBoxLayout *syncLayout = new QHBoxLayout;

//    loginLayout->addWidget(new QLabel(tr("Username:")));
//    loginLayout->addWidget(m_usernameEdit);
//    loginLayout->addWidget(new QLabel(tr("Password:")));
//    loginLayout->addWidget(m_passwordEdit);
//    loginLayout->addWidget(new QLabel(tr("Repository:")));
//    loginLayout->addWidget(m_repoEdit);
//    loginLayout->addWidget(m_loginBtn);

//    mainLayout->addLayout(loginLayout);
//    mainLayout->addWidget(m_statusLabel);

//    syncLayout->addWidget(m_syncToLocalBox);
//    syncLayout->addWidget(m_syncToRemoteBox);
//    syncLayout->addWidget(m_syncBtn);
//    syncLayout->addWidget(m_progressBar);

//    mainLayout->addLayout(syncLayout);

//    setLayout(mainLayout);

//    // 连接槽函数
//    connect(m_loginBtn, &QPushButton::clicked, this, &GitSync::login);
//    connect(m_syncBtn, &QPushButton::clicked, this, &GitSync::syncToLocal);
//    connect(m_syncBtn, &QPushButton::clicked, this, &GitSync::syncToRemote);
}

GitSync::~GitSync()
{
}



void GitSync::login(QString url)
{
//    QString url = QInputDialog::getText(this,
//                                                 "QInputdialog_Name",
//                                                 "请输入github仓库地址",
//                                                 QLineEdit::Normal,
//                                                 "note"
//                                                 );
    if(url!=nullptr)
    {
        m_repoUrl=url;

        // 执行git clone命令，将远程仓库克隆到本地
        QProcess process;
        process.start("git clone " + m_repoUrl);
        process.waitForFinished(-1);

        // 检查git clone命令是否执行成功
        if (process.exitCode() != 0)
        {

            //QMessageBox::warning(this,tr("失败"),tr("Failed to clone repository."));
            return;
        }
        //QMessageBox::warning(this,tr("Successfully"),tr("Successfully logged in."));

    }
    else {
        //QMessageBox::warning(this,tr("失败"),tr("失败"));
    }

//    m_username = m_usernameEdit->text();
//    m_password = m_passwordEdit->text();
//    m_repoUrl = m_repoEdit->text();

//    // 检查用户名、密码和仓库地址是否为空
//    if (m_username.isEmpty() || m_password.isEmpty() || m_repoUrl.isEmpty())
//    {
//        QMessageBox::warning(this, tr("Warning"), tr("Username, password and repository url are required."));
//        return;
//    }


//    // 执行git clone命令，将远程仓库克隆到本地
//    QProcess process;
//    process.start("git clone " + m_repoUrl);
//    process.waitForFinished(-1);

//    // 检查git clone命令是否执行成功
//    if (process.exitCode() != 0)
//    {
//        m_statusLabel->setText(tr("Failed to clone repository."));
//        return;
//    }

//    m_statusLabel->setText(tr("Successfully logged in."));


}



//void GitSync::syncToLocal()
//{
//    if (!m_syncToLocalBox->isChecked())
//        return;

//    // 执行git pull命令，将本地仓库与远程仓库同步
//    QProcess process;
//    //process.setWorkingDirectory("repo");  //设置工作目录为本地仓库的路径

//    /////// 进入指定文件夹
//        //QDir::setCurrent("./testGit");
//    QString str = m_repoUrl;
//    QStringList list = str.split(QRegExp("[/\\\\]"));
//    QString repoName = list.last().remove(".git");
//    qDebug() << repoName;

//    QDir::setCurrent("./"+repoName);

////////
//m_repoName=repoName;

//    process.start("git pull");
//    process.waitForFinished(-1);

//// 检查git pull命令是否执行成功
//    if (process.exitCode() != 0)
//    {
//        m_statusLabel->setText(tr("Failed to sync to local."));
//        return;
//    }

//    m_statusLabel->setText(tr("Successfully synced to local."));
//    }

//void GitSync::syncToRemote()
//{
//    if (!m_syncToRemoteBox->isChecked())
//    return;

//    // 执行git add命令，将本地文件添加到暂存区
//    QProcess process;
//    //process.setWorkingDirectory("repo");  // 设置工作目录为本地仓库的路径

//    /////// 进入指定文件夹
//        //QDir::setCurrent("./testGit");
//        QString str = m_repoUrl;
//        QStringList list = str.split(QRegExp("[/\\\\]"));
//        QString repoName = list.last().remove(".git");
//        qDebug() << repoName;
//        QDir::setCurrent("./"+repoName);

//    process.start("git add .");  // 添加所有文件到暂存区
//    process.waitForFinished(-1);

//    // 检查git add命令是否执行成功
//    if (process.exitCode() != 0)
//    {
//        m_statusLabel->setText(tr("Failed to add files."));
//        return;
//    }

//    // 执行git commit命令，提交更改到本地仓库
//    process.start("git commit -m \"Sync changes\"");
//    process.waitForFinished(-1);

//    // 检查git commit命令是否执行成功
//    if (process.exitCode() != 0)
//    {
//        m_statusLabel->setText(tr("Failed to commit changes."));
//        return;
//    }

//    // 执行git push命令，将本地仓库的更改推送到远程仓库
//    process.start("git push");
//    connect(&process, &QProcess::readyReadStandardError, [&]() {
//        QString output = QString::fromLocal8Bit(process.readAllStandardError());
//        qDebug() << output;

//    // 检查push过程中是否需要在终端输入密码
//    if (output.contains("Username for"))
//    {
//        process.write(m_username.toUtf8() + "\n");
//        process.waitForBytesWritten();
//        process.waitForReadyRead();
//        process.write(m_password.toUtf8() + "\n");
//        process.waitForBytesWritten();
//    }
//    });
//    process.waitForFinished(-1);

//    // 检查git push命令是否执行成功
//    if (process.exitCode() != 0)
//    {
//        m_statusLabel->setText(tr("Failed to push changes."));
//        return;
//    }

//    m_statusLabel->setText(tr("Successfully synced to remote."));
//}

//void GitSync::updateProgress(int value)
//{
//    m_progressBar->setValue(value);
//}
