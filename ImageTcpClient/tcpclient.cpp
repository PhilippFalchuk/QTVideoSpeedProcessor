#include "tcpclient.h"

#include <QHostAddress>


TcpClient::TcpClient(QObject *parent)
    : QObject{parent},
      m_socket(this)
{
    m_socket.connectToHost(QHostAddress("127.0.0.1"), 4242);
    connect(&m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
}

void TcpClient::onReadyRead()
{

//    QImage recievedImage ( (uchar*)m_socket.readAll().data(), 4000,720, QImage::Format_RGB32);

//    emit gotImage(recievedImage);

    bool flag;
    QByteArray ba;
    QDataStream in(ba);
    QImage temp;



}
