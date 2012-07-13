#ifndef DOWNLOADPROGRESSDIALOG_H
#define DOWNLOADPROGRESSDIALOG_H

#include <QProgressDialog>
#include <QList>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DownloadProgressDialog : public QProgressDialog
{
    Q_OBJECT
public:
    explicit DownloadProgressDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    void AddUrls(const QList<QUrl> &urls);

private:
    QNetworkAccessManager networkManager;
    QNetworkReply *reply;
    quint32 urlNumber;
    quint32 finishedDownload;
    QList<QNetworkReply*> listReply;
signals:
    void downloadFinished(QList<QNetworkReply*> listReply);

private slots:
    void httpFinished(QNetworkReply* reply);
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void DialogCancel();
};

#endif // DOWNLOADPROGRESSDIALOG_H
