#include "newplayer.h"
#include "ui_newplayer.h"
#include "videowidget.h"
#include "videosurface.h"
#include <QtMath>
#include <QSerialPortInfo>
#include <QFile>

NewPlayer::NewPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewPlayer)
{
    ui->setupUi(this);


    m_videoWidget = new VideoWidget();
    m_videoWidget->resize(200,200);

    VideoSurface* videoSurface = new VideoSurface;


    m_player = new QMediaPlayer;
    m_player->setVideoOutput(/*m_videoWidget*/ videoSurface);

    ui->horizontalLayout->addWidget(m_videoWidget);
    m_videoWidget->setVisible(true);

    m_threadHandler = new ThreadHandler();
//    ui->verticalLayout->addWidget(m_fpsWidget);

    m_videoProbe = new QVideoProbe(this);

    connect(videoSurface, &VideoSurface::frameAvailable, m_threadHandler, &ThreadHandler::processFrame);
    //    connect(m_videoProbe, &QVideoProbe::videoFrameProbed, m_threadHandler, &ThreadHandler::processFrame);

    //connect(m_threadHandler, SIGNAL(updatelineedit(int)), this, SLOT(updatelineedit(int)));
    m_videoProbe->setSource(m_player);

    connect(this, SIGNAL(zoneChanged(int,int,int, bool)), m_threadHandler,SLOT(setZone(int,int,int, bool)));


    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        ui->comboBox->addItem(info.portName());

    connect(this, &NewPlayer::portChanged, &m_comThread, &ComPortThread::changePort);

    connect(&m_threadHandler->m_processor, &FrameProcessor::shiftReady, &m_comThread, &ComPortThread::sendShift);
    //connect(&m_threadHandler->m_processor, &FrameProcessor::shiftReady, this, &NewPlayer::onlyTextShift);
    connect(&m_threadHandler->m_processor, &FrameProcessor::imageReady, &m_imageTcpServer, &ImageTcpServer::writeImageToClient);


    QFile fileIn(m_pathToUrl);

    if(fileIn.open(QIODevice::ReadOnly))
    {
        QTextStream in(&fileIn);
        while(!fileIn.atEnd())
        {
            QString line = in.readLine();
            m_url = line;
        }
    }
    //qDebug()<< QDir::currentPath();
    qDebug()<< m_pathToUrl;
    qDebug()<< m_url;

    ui->pushButton_2->setVisible(false);
}

NewPlayer::~NewPlayer()
{
    delete ui;
}


void NewPlayer::on_pushButton_clicked()
{
    if(QFile::exists(m_pathToUrl) && m_url != "")
    {
    m_player->setMedia(QUrl(m_url));
    m_player->play();
    }
    else
    {
        m_player->setMedia(QUrl("rtsp://user:h5106120@192.168.0.228:554/ISAPI/Streaming/Channels/101"));
        m_player->play();
    }
//    QPushButton *btn = new QPushButton(m_videoWidget);
//    btn->setText("sdfasfasdfd\n\n\n\n\n\n\nfsadfasdfasffda");
//    btn->setVisible(true);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(true);
}


void NewPlayer::on_pushButton_2_clicked()
{
    m_player->setMedia(QMediaContent());
    ui->pushButton_2->setVisible(false);
    ui->pushButton->setVisible(true);
}

void NewPlayer::on_editZone_clicked()
{
    int width = ui->spinBox->value();
    int height = ui->spinBox_2->value();
    int widthOfDis = ui->spinBox_3->value();
    bool saverFlag = ui->spinBox_saver->value();

    emit zoneChanged(width,height, widthOfDis, saverFlag);
}

void NewPlayer::on_portPushButton_clicked()
{
    emit portChanged(ui->comboBox->currentText());
}

