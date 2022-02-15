#ifndef TCPCLIENWIMDOW_H
#define TCPCLIENWIMDOW_H

#include "tcpclient.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TcpClienWimdow; }
QT_END_NAMESPACE

class TcpClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    TcpClientWindow(QWidget *parent = nullptr);
    ~TcpClientWindow();

public slots:
    void onImageRecieved(QImage image);

private:
    Ui::TcpClienWimdow *ui;

    TcpClient m_tcpClient;
};
#endif // TCPCLIENWIMDOW_H
