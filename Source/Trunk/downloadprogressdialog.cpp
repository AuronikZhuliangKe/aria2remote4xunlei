#include "downloadprogressdialog.h"

DownloadProgressDialog::DownloadProgressDialog(QWidget * parent, Qt::WindowFlags f) :
      QProgressDialog(parent,f)
    , urlNumber(0)
    , finishedDownload(0)
{
    setMinimum(0);
    setAutoClose(true);
    setAutoReset(true);

    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpFinished(QNetworkReply*)));
    connect(this, SIGNAL(canceled()), this, SLOT(DialogCancel()));
 }

void DownloadProgressDialog::AddUrls(const QList<QUrl> &urls)
{
    urlNumber = urls.size();
    foreach (QUrl url, urls)
    {
        reply = networkManager.get(QNetworkRequest(url));
        listReply.append(reply);
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(updateDataReadProgress(qint64,qint64)));
    }
}

void DownloadProgressDialog::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (totalBytes != -1)
    {
        if (urlNumber == 1)
        {
            setValue(bytesRead);
            setMaximum(totalBytes);
        }
        else
        {
            setValue(finishedDownload);
            setMaximum(urlNumber);
        }
    }
}

void DownloadProgressDialog::httpFinished(QNetworkReply* reply)
{
    if (!wasCanceled())
    {
        if (urlNumber == 1)
        {
            emit downloadFinished(listReply);
        }
        else
        {
            finishedDownload++;
            listReply.append(reply);
            if (finishedDownload == urlNumber)
            {
                emit downloadFinished(listReply);
            }
        }
    }
}

void DownloadProgressDialog::DialogCancel()
{
    foreach (QNetworkReply *r, listReply)
    {
        r->abort();
        r->deleteLater();
    }
}
