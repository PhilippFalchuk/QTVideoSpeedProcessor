#include "tcpclientwindow.h"
#include "ui_tcpclienwimdow.h"
#include <QDir>
#include <QDateTime>
#include <QPainter>

TcpClientWindow::TcpClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClienWimdow)
{
    ui->setupUi(this);

    connect(&m_tcpClient, &TcpClient::gotImage, this, &TcpClientWindow::onImageRecieved);



     qDebug() << QDateTime::currentDateTime().toString("ss:zzz");





}

TcpClientWindow::~TcpClientWindow()
{
    delete ui;
}

void TcpClientWindow::onImageRecieved(QImage image)
{
    qDebug() << image;
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    QString str = QDir::homePath() + "/recievedImages/" + QDateTime::currentDateTime().toString("hh'h'mm'm'ss's'zzz'ms'") + ".bmp";
    qDebug() << str;
    image.save(str, "bmp");
}

