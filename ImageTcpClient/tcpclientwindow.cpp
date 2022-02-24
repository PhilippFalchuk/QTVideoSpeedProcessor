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

    connect(this, &TcpClientWindow::tcpStartButtonClicked, &m_tcpClient, &TcpClient::formConnection);
    connect(this, &TcpClientWindow::tcpStopButtonClicked, &m_tcpClient, &TcpClient::toDisconnect);




     qDebug() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");



    ui->pushButton_2->setVisible(false);

}

TcpClientWindow::~TcpClientWindow()
{
    delete ui;
}

void TcpClientWindow::onImageRecieved(QImage image)
{
    QImage tempImage = QImage(widthOfCombinedImage + image.width(), image.height(), QImage::Format_RGB32);

    QPainter painter(&tempImage);

//    painter.drawImage(0,0, m_combinedImage);
//    painter.drawImage(m_combinedImage.width(), 0, image);

    painter.drawImage(tempImage.width() - m_combinedImage.width(),0, m_combinedImage);
    painter.drawImage(tempImage.width() - m_combinedImage.width() - image.width(), 0, image);

    m_combinedImage = tempImage;
    widthOfCombinedImage = m_combinedImage.width();


    if(m_combinedImage.width() < 1000){
        qDebug() << m_combinedImage;
        ui->imageLabel->setPixmap(QPixmap::fromImage(m_combinedImage));
        //QString str = QDir::homePath() + "/recievedImages/" + QDateTime::currentDateTime().toString("hh'h'mm'm'ss's'zzz'ms'") + ".bmp";
        //qDebug() << str;
        //m_combinedImage.save(str, "bmp");
    }else{
        m_combinedImage = QImage();
        widthOfCombinedImage = 0;
    }
}


void TcpClientWindow::on_pushButton_clicked()
{
    ui->imageLabel->setPixmap(QPixmap::fromImage(QImage()));

    emit tcpStartButtonClicked();
    ui->pushButton_2->setVisible(true);
    ui->pushButton->setVisible(false);
}


void TcpClientWindow::on_pushButton_2_clicked()
{
    emit tcpStopButtonClicked();
    ui->pushButton_2->setVisible(false);
    ui->pushButton->setVisible(true);
}

