#include "tcpclient.h"

#include <QHostAddress>
#include <QTimer>


TcpClient::TcpClient(QObject *parent)
    : QObject{parent},
      m_socket(this)
{
    m_socket.connectToHost(QHostAddress("127.0.0.1"), 4242);
    connect(&m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
}

void TcpClient::onReadyRead()
{

//    qDebug() << m_socket.bytesAvailable() << " " << QImage(4000,720,QImage::Format_RGB32).sizeInBytes();

//    if(m_socket.bytesAvailable() >= QImage(4000,720,QImage::Format_RGB32).sizeInBytes())
//    {
//        qDebug() << "started image conversion";
//        //QImage recievedImage(4000,720,QImage::Format_RGB32);
//        QByteArray ba;
//        ba = m_socket.readAll();

//        QImage image((uchar*)ba.data(), 4000,720, QImage::Format_RGB32);
//        emit gotImage(image);

//    }





    QDataStream in(&m_socket);


    if(sizeOfImageInBytes == 0){
        if(m_socket.bytesAvailable() < sizeof(quint32))
            return;
        in >> sizeOfImageInBytes;
        qDebug() << "size recieved " << sizeOfImageInBytes;
    }


    if(m_socket.bytesAvailable() < sizeOfImageInBytes)
        return;

    QImage im;

    in >> im;

    emit gotImage(im);

    sizeOfImageInBytes = 0;




}


