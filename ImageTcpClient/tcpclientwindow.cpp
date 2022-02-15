#include "tcpclientwindow.h"
#include "ui_tcpclienwimdow.h"

TcpClientWindow::TcpClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClienWimdow)
{
    ui->setupUi(this);

    connect(&m_tcpClient, &TcpClient::gotImage, this, &TcpClientWindow::onImageRecieved);

            ui->imageLabel->setBackgroundRole(QPalette::Dark);
            ui->imageLabel->setScaledContents(true);
}

TcpClientWindow::~TcpClientWindow()
{
    delete ui;
}

void TcpClientWindow::onImageRecieved(QImage image)
{
    qDebug() << image;
    //ui->imageLabel->setPixmap(QPixmap::fromImage(image));
}

