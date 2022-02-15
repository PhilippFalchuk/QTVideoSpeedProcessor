#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

public slots:
    void onReadyRead();

private:
    QTcpSocket m_socket;

};

#endif // TCPCLIENT_H
