// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2009 Dmytro Poplavskiy <dmitry.poplavsky@gmail.com>

#include "client.h"
#include "request.h"
#include "response.h"

#include <QtGui/QApplication>
#include <qhttp.h>
#include <qbuffer.h>
#include <QtNetwork>
#include <QAuthenticator>
#include "util.h"

namespace  xmlrpc {

class Client::Private
{
public:
    QString hostName;
    quint16 port;
    QString path;

    QString userName;
    QString password;

    QString userAgent;

    QHttp *http;

    QAuthenticator proxyAuth;

    QMap<int,QBuffer*> serverResponses;
    int iTypes;

    bool bGZip;

    //current selected download item
    qint64 iGID;
};

/**
 * Constructs a XmlRPC client.
 */
Client::Client(QObject * parent)
: QObject( parent ),
  m_bHeaderGZip(false)
{
    d = new Private;
    d->port = 6800;
    d->path = "/rpc";
    d->userAgent = "Aria2Remote";
    d->http = new QHttp(this);

    connect( d->http, SIGNAL(requestFinished(int,bool)), SLOT(requestFinished(int,bool)) );

    connect( d->http, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),
             this, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *) ) );

    connect( d->http, SIGNAL( authenticationRequired ( const QString &, quint16, QAuthenticator * ) ),
             this, SIGNAL( authenticationRequired ( const QString &, quint16, QAuthenticator * ) ) );
}

/**
 * Constructs a XmlRPC client for communication with XmlRPC
 * server running on host \a hostName \a port.
 */
Client::Client(const QString & hostName, quint16 port, QObject * parent)
: QObject( parent )
{
    d = new Private;
    setHost( hostName, port );

    //important: dissconnect all connection from http in destructor,
    //otherwise crashes are possible when other parts of Client::Private
    //is deleted before http
    connect( d->http, SIGNAL(requestFinished(int,bool)), SLOT(requestFinished(int,bool)) );
}

/**
 * Destroys the XmlRPC client.
 */
Client::~Client()
{
    // it's necessary to delete QHttp instance before Private instance
    // to be sure Client slots will not be called with already deleted Private data
    delete d->http;
    qDeleteAll( d->serverResponses );
    delete d;
}


/**
 * Sets the XML-RPC server that is used for requests to hostName
 * on port \a port and path \path.
 */
void Client::setHost( const QString & hostName, quint16 port, QString path )
{
    d->hostName = hostName;
    d->port = port;
    d->path = path;
    d->http->setHost( hostName, port );
}

/**
 * Enables HTTP proxy support, using the proxy server host on port port.
 * username and password can be provided if the proxy server
 * requires authentication.
 */
void Client::setProxy( const QString & host, int port, 
                    const QString & userName, const QString & password )
{

#ifdef QT_DEBUG
    qDebug() << "xmlrpc client: set proxy" << host << port << userName << password;
#endif

    d->http->setProxy( host, port, userName, password );
}

/**
 * Replaces the internal QTcpSocket that QHttp uses with socket.
 * This can be useful for adding https support with QtSslSocket.
 * 
 * Check
 * \l{http://trolltech.com/products/qt/addon/solutions/catalog/4/Utilities/qtsslsocket/}
 * and QHttp::setSocket() for more information.
 */
void Client::setSocket( QTcpSocket * socket )
{
    d->http->setSocket( socket );
}

/**
 * Set the user name userName and password password for XML-RPC
 * ( or http ) server that require authentication.
 */
void Client::setUser( const QString & userName, const QString & password )
{
    //d->http->setUser( userName, password );
    d->userName = userName;
    d->password = password;
}

/**
 * Set the user agent HTTP value instead of default "QXMLRPC"
 */
void Client::setUserAgent( const QString & userAgent )
{
    d->userAgent = userAgent;
}

/**
 * Call method methodName on server side with parameters list
 * params. Returns id of request, used in done() and failed()
 * signals.
 * 
 * The parameters order is changed in overloaded methods to
 * avoid situation when the only parameter is the list.
 * \code
 * QList<xmlrpc::Variant> parameter;
 * ...
 * int requestId = client->request( methodName, parameter );
 * \endcode
 * This leads to this method be called, with parameter treated
 * as parameters list. It's possible to fix this with next code:
 * \code
 * client->request( methodName, xmlrpc::Variant(parameter) );
 * \endcode
 * but to avoid such kind of bugs, the parameters order in
 * overloaded methods was changed.
 */
int Client::request( QList<Variant> &params, QString methodName, int iTypes, qint64 iGID )
{
    QBuffer *outBuffer = new QBuffer;

    QByteArray data = Request(methodName,params).composeRequest();

    QHttpRequestHeader header("POST",d->path);
    header.setContentLength( data.size() );
    header.setContentType("text/xml");
    header.setValue( "User-Agent", d->userAgent );
    header.setValue("Accept","*/*");

    if (m_bHeaderGZip)
    {
        header.setValue("Accept-Encoding", "gzip");
    }

    if ( !d->userName.isEmpty() ) {
        QByteArray authData = QString(d->userName + ":" + d->password).toUtf8();
        authData = authData.toBase64();
        authData = QByteArray("Basic ")+authData;
        header.setValue("Authorization", authData.data() );
    }

    header.setValue("host",d->hostName + QString(":") + QString::number(d->port));

    int id = d->http->request( header, data, outBuffer );
    d->serverResponses[id] = outBuffer;
    d->iTypes = iTypes;
    d->iGID = iGID;
    d->bGZip = m_bHeaderGZip;
    d->http->close();

#ifdef QT_DEBUG
    QFile RequestWriteToFile(util::getHomePath() + "aria_request.xml");
    if (RequestWriteToFile.open(QIODevice::ReadWrite))
    {
        RequestWriteToFile.seek(RequestWriteToFile.size());
        RequestWriteToFile.write(Request(methodName,params).composeRequest(1));
        RequestWriteToFile.close();
    }

    qDebug() << "xmlrpc request(" << id << "): " << methodName;
    qDebug() << Variant(params).pprint();
#endif

    return id;
}

/**
 * Call method methodName on server side with empty parameters
 * list. This is an overloaded member function, provided for
 * convenience.
 */
int Client::request( QString methodName )
{
    QList<xmlrpc::Variant> params;
    return request( params, methodName, 0 );
}

/**
 * Call method methodName on server side with one parameter.
 * This is an overloaded member function, provided for
 * convenience.
 */
int Client::request( QString methodName, Variant param1 )
{
    QList<xmlrpc::Variant> params;
    params << param1;
    return request( params, methodName, 0 );
}

/**
 * Call method methodName on server side with two parameters.
 * This is an overloaded member function, provided for
 * convenience.
 */
int Client::request( QString methodName, Variant param1, Variant param2 )
{
    QList<xmlrpc::Variant> params;
    params << param1 << param2;
    return request( params, methodName, 0 );
}

/**
 * Call method methodName on server side with three parameters.
 * This is an overloaded member function, provided for
 * convenience.
 */
int Client::request( QString methodName, Variant param1, Variant param2, Variant param3 )
{
    QList<xmlrpc::Variant> params;
    params << param1 << param2 << param3;
    return request( params, methodName, 0 );
}
/**
 * Call method methodName on server side with four parameters.
 * This is an overloaded member function, provided for
 * convenience.
 */
int Client::request( QString methodName, Variant param1, Variant param2, Variant param3, Variant param4 )
{
    QList<xmlrpc::Variant> params;
    params << param1 << param2 << param3 << param4;
    return request( params, methodName, 0 );
}

void Client::requestFinished(int id, bool error)
{
    if ( !d->serverResponses.count(id) ) {
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "request" <<  "finished, id=" << id << ", isError:" << error;
#endif

    if ( error ) {
        QBuffer *buffer = d->serverResponses.take(id);
        delete buffer;

        emit failed(id, d->http->error(), d->http->errorString() );
        return;
    }

    if ( d->serverResponses.count(id) ) {
        QBuffer *buffer = d->serverResponses.take(id);
        QByteArray buf = buffer->buffer();

        if (d->bGZip)
        {
            #ifdef QT_DEBUG
                QFile debug_writeToFile(util::getHomePath() + "compressed_response.dat");
                if (debug_writeToFile.open(QIODevice::ReadWrite))
                {
                    debug_writeToFile.seek(debug_writeToFile.size());
                    debug_writeToFile.write(buf);
                    debug_writeToFile.close();
                }
            #endif
            buf = util::gzipDecompress(buf);
            #ifdef QT_DEBUG
                QFile debug_writeToFile1(util::getHomePath() + "uncompressed_response.dat");
                if (debug_writeToFile1.open(QIODevice::ReadWrite))
                {
                    debug_writeToFile1.seek(debug_writeToFile1.size());
                    debug_writeToFile1.write(buf);
                    debug_writeToFile1.close();
                }
            #endif
        }

        Response response;
        QString errorMessage;

        if (buf.size() == 0)
        {
            delete buffer;
            return;
        }

        if ( response.setContent( buf, &errorMessage ) ) {
            Q_ASSERT( !response.isNull() );

            if ( response.isFault() ) {
                qDebug() << "request failed:" << response.faultCode() << response.faultString();
                emit failed(id, response.faultCode(), response.faultString() );
            } else {
#ifdef QT_DEBUG
                qDebug() << response.returnValue().pprint();
#endif
                emit done( d->iTypes, d->iGID, id, response.returnValue() );
            }

        } else {

#ifdef QT_DEBUG
            qDebug() << "incorrect xmlrpc response:" << errorMessage;
            qDebug() << QString(buf);
#endif
            emit failed(id, -32600, "Server error: Invalid xml-rpc. \nNot conforming to spec.");
        }
        delete buffer;

    }
	
}

} 
