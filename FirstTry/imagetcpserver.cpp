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
    qDebug() << "server created " << m_server.serverAddress() << " server is listening: " << m_server.isListening();
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
    if (socketState == QAbstractSocket::UnconnectedState){
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        m_sockets.removeOne(sender);
    }
}

void ImageTcpServer::onReadyRead()
{

}

void ImageTcpServer::writeImageToClient(QImage dynamicImage)
{

    qDebug() << dynamicImage;

    if(!(m_sockets.isEmpty())){
        QByteArray block;
        QDataStream data(&block, QIODevice::WriteOnly);
        data.setVersion(QDataStream::Qt_5_12);

        data << dynamicImage << bool{1};



        for (QTcpSocket* socket : m_sockets){
            socket->write(block);
            socket->waitForBytesWritten(-1);
        }
    }else{
        qDebug() << "there are no clients connected currently";
    }
}
