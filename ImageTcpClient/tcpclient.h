#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QImage>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

public slots:
    void onReadyRead();

signals:
    void gotImage(QImage recievedImage);

private:
    QTcpSocket m_socket;

};

#endif // TCPCLIENT_H
