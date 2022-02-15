#include "imagetcpserver.h"
#include <QDebug>
#include <QHostAddress>
#include <QAbstractSocket>

ImageTcpServer::ImageTcpServer(QObject *parent)
    : QObject{parent},
      m_server(this)
{
    m_server.listen(QHostAddress::Any, 4242);
    connect(&m_server, &QTcpServer::newConnection, this, &ImageTcpServer::onNewConnection);
    qDebug() << "server created " << m_server.serverAddress();
}

void ImageTcpServer::onNewConnection()
{
    qDebug() << "new connection";
    QTcpSocket *clientSocket = m_server.nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &ImageTcpServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::stateChanged, this, &ImageTcpServer::onSocketStateChanged);

    m_sockets.push_back(clientSocket);
    for (QTcpSocket* socket : m_sockets){
        socket->write(QByteArray::fromStdString(clientSocket->peerAddress().toString().toStdString() + " connected to server !\n"));
    }
}

void ImageTcpServer::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        m_sockets.removeOne(sender);
    }
}

void ImageTcpServer::onReadyRead()
{

}

void ImageTcpServer::writeToClient(int shift)
{
    if(!(m_sockets.isEmpty())){
        for (QTcpSocket* socket : m_sockets){
            socket->write(QByteArray::fromStdString(QString::number(shift).toStdString()));
        }
    }else{
        qDebug() << "there are no clients connected currently";
    }
}
