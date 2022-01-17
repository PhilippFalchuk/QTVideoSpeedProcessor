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

    m_series = new QLineSeries();
    m_series->append(0, 6.7);
    m_series->append(40, 48);



    m_graphChartDerivative = new QChart();
    m_graphChartDerivative->legend()->hide();
    m_graphChartDerivative->createDefaultAxes();
    m_graphChartDerivative->addSeries(m_series);
    m_graphChartDerivative->setTitle("derivative");

    m_graphChartDiscrepancy = new QChart();
    m_graphChartDiscrepancy->legend()->hide();
    m_graphChartDiscrepancy->createDefaultAxes();

    m_graphChartDiscrepancy->setTitle("discrepancy");

    QChartView *graphChartViewDerivative = new QChartView(m_graphChartDerivative);
    QChartView *graphChartViewDiscrepancy = new QChartView(m_graphChartDiscrepancy);
    //graphChartViewDerivative->setRenderHint(QPainter::Antialiasing);

    //ui->horizontalLayout->addWidget(graphChartViewDerivative);
    ui->gridLayout->addWidget(graphChartViewDiscrepancy,1,1);
    ui->gridLayout->addWidget(graphChartViewDerivative,2,1);


    m_videoProbe = new QVideoProbe(this);
    connect(m_videoProbe, &QVideoProbe::videoFrameProbed, m_fpsWidget, &FPSWidget::processFrame);
    connect(m_fpsWidget, SIGNAL(updatelineedit(int)), this, SLOT(updatelineedit(int)));
    m_videoProbe->setSource(m_player);

    connect(m_fpsWidget, SIGNAL(frameReady(QVector<double>, QVector<double>)), this, SLOT(processChart(QVector<double>, QVector<double>)));





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

void NewPlayer::processChart(QVector<double> graphDerivative, QVector<double> graphDiscrepancy)
{
    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();

    for(int counter = 0; counter < graphDerivative.size(); counter++)
    {
        series1->append(counter,graphDerivative[counter]);
    }
    for(int counter = 0; counter < graphDiscrepancy.size(); counter++)
    {
        series2->append(counter,graphDiscrepancy[counter]);
    }

    m_graphChartDerivative->removeAllSeries();
    m_graphChartDiscrepancy->removeAllSeries();
    m_graphChartDerivative->addSeries(series1);
    m_graphChartDiscrepancy->addSeries(series2);

}


