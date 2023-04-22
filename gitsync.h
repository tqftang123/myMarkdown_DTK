#ifndef GITSYNC_H
#define GITSYNC_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QProgressBar>

class GitSync : public QWidget
{
    Q_OBJECT

public:
    explicit GitSync(QWidget *parent = nullptr);
    ~GitSync();
void login(QString url);

//private slots:

//    void syncToLocal();
//    void syncToRemote();
//    void updateProgress(int value);

private:
//    QLineEdit *m_usernameEdit;
//    QLineEdit *m_passwordEdit;
//    QLineEdit *m_repoEdit;
//    QPushButton *m_loginBtn;
//    QLabel *m_statusLabel;
//    QCheckBox *m_syncToLocalBox;
//    QCheckBox *m_syncToRemoteBox;
//    QPushButton *m_syncBtn;
//    QProgressBar *m_progressBar;

    QString m_username;
    QString m_password;
    QString m_repoUrl;
public:
    QString m_repoName;
};

#endif // GITSYNC_H
