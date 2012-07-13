#ifndef TORRENTCLIENT_H
#define TORRENTCLIENT_H
#include <QString>
#include <QRegExp>
#include <QMap>
#include <QChar>
#include <QtGui/QApplication>

class TorrentClient
{
    Q_DECLARE_TR_FUNCTIONS(TorrentClient)
public:
    TorrentClient();
    QString getClientName(const QString &origStr);

private:
    QMap<QString,QString> azLikeClients4;
    QMap<QString,QString> azLikeClients3;
    QMap<QString,QString> azLikeClients2x2;
    QMap<QString,QString> azLikeClientsSpec;
    QMap<QString,QString> shLikeClients;

    QString getMnemonicEnd( char ch );
    QString shChar( QChar ch );
};

#endif // TORRENTCLIENT_H
