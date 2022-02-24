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

signals:
    void tcpStartButtonClicked();
    void tcpStopButtonClicked();

public slots:
    void onImageRecieved(QImage image);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::TcpClienWimdow *ui;

    TcpClient m_tcpClient;
    QImage m_combinedImage = QImage();
};
#endif // TCPCLIENWIMDOW_H
