#ifndef IMAGETCPSERVER_H
#define IMAGETCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QImage>

class ImageTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit ImageTcpServer(QObject *parent = nullptr);

public slots:
    void onNewConnection();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();
    void writeImageToClient(QImage dynamicImage);

private:
    QTcpServer  m_server;
    QList<QTcpSocket*>  m_sockets;

};

#endif // IMAGETCPSERVER_H
