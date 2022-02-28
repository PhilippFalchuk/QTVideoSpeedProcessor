#include "tcpclient.h"

#include <QHostAddress>
#include <QTimer>


TcpClient::TcpClient(QObject *parent)
    : QObject{parent},
      m_socket(this)
{

    connect(&m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
}

void TcpClient::onReadyRead()
{
    QDataStream in(&m_socket);

    if(sizeOfImageInBytes == 0){
        if(m_socket.bytesAvailable() < sizeof(quint32))
            return;
        in >> sizeOfImageInBytes;
        //qDebug() << "size recieved " << sizeOfImageInBytes;
    }

    if(m_socket.bytesAvailable() < sizeOfImageInBytes)
        return;

    QImage im;

    in >> im;

    emit gotImage(im);

    sizeOfImageInBytes = 0;
}

void TcpClient::formConnection()
{
    m_socket.connectToHost(QHostAddress("127.0.0.1"), 4242);
}

void TcpClient::toDisconnect()
{
    m_socket.disconnectFromHost();
}


