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

}
