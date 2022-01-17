#include "newplayer.h"
#include "ui_newplayer.h"
#include "videowidget.h"

NewPlayer::NewPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewPlayer)
{
    ui->setupUi(this);
    m_videoWidget = new VideoWidget(this);
    m_videoWidget->resize(200,200);

    m_player = new QMediaPlayer;
    m_player->setVideoOutput(m_videoWidget);

    m_fpsWidget = new FPSWidget();
//    ui->verticalLayout->addWidget(m_fpsWidget);

    QLineSeries *series = new QLineSeries();
    series->append(0, 6.7);
    series->append(40, 48);



    m_graphChart = new QChart();
    m_graphChart->legend()->hide();
    m_graphChart->createDefaultAxes();
    m_graphChart->addSeries(series);
    m_graphChart->setTitle("graph");

    QChartView *graphChartView = new QChartView(m_graphChart);
    graphChartView->setRenderHint(QPainter::Antialiasing);

    ui->horizontalLayout->addWidget(graphChartView);


    m_videoProbe = new QVideoProbe(this);
    connect(m_videoProbe, &QVideoProbe::videoFrameProbed, m_fpsWidget, &FPSWidget::processFrame);
    connect(m_fpsWidget, SIGNAL(updatelineedit(int)), this, SLOT(updatelineedit(int)));
    m_videoProbe->setSource(m_player);

    connect(m_fpsWidget, SIGNAL(frameReady(QVector<double>)), this, SLOT(processChart(QVector<double>)));
    connect(m_fpsWidget, SIGNAL(sendGraph(QVector<double>)), this, SLOT(processChart(QVector<double>)));





}

NewPlayer::~NewPlayer()
{
    delete ui;
}


void NewPlayer::on_pushButton_clicked()
{
    m_player->setMedia(QUrl("rtsp://user:h5106120@192.168.0.228:554/ISAPI/Streaming/Channels/101"));
    m_player->play();
}


void NewPlayer::on_pushButton_2_clicked()
{
    m_player->setMedia(QMediaContent());
}
void NewPlayer::updatelineedit(int a)
{
    QString str = QString::number(a);
    ui->lineEdit->setText(str);
}

void NewPlayer::processChart(QVector<double> graphBWA)
{
    QLineSeries *series = new QLineSeries();

    for(int counter = 0; counter < graphBWA.size(); counter++)
    {
        series->append(counter,graphBWA[counter]);
    }
    m_graphChart->removeAllSeries();
    m_graphChart->addSeries(series);

}


