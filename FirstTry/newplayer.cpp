#include "newplayer.h"
#include "ui_newplayer.h"
#include "videowidget.h"
#include <QtMath>

NewPlayer::NewPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewPlayer)
{
    ui->setupUi(this);
    m_videoWidget = new VideoWidget(this);
    m_videoWidget->resize(200,200);

    m_player = new QMediaPlayer;
    m_player->setVideoOutput(m_videoWidget);

    ui->horizontalLayout->addWidget(m_videoWidget);

    m_fpsWidget = new FPSWidget();
//    ui->verticalLayout->addWidget(m_fpsWidget);

    m_series = new QLineSeries();
    m_series->append(0, 6.7);
    m_series->append(40, 48);

    m_bufferSeries = new QLineSeries();
//    m_bufferSeries->append(0, 0);
//    m_bufferSeries->append(2, 2);




    m_graphChartDerivative = new QChart();
    m_graphChartDerivative->legend()->hide();
    m_graphChartDerivative->createDefaultAxes();
    m_graphChartDerivative->addSeries(m_series);
    m_graphChartDerivative->setTitle("derivative");

    m_graphChartDiscrepancy = new QChart();
    m_graphChartDiscrepancy->legend()->hide();
    m_graphChartDiscrepancy->createDefaultAxes();
    m_graphChartDiscrepancy->setTitle("discrepancy");

    m_graphChartPreviousDerivative =new QChart();
    m_graphChartPreviousDerivative->legend()->hide();
    m_graphChartPreviousDerivative->createDefaultAxes();
    m_graphChartPreviousDerivative->setTitle("derivative2");

    m_bufferChart =new QChart();
    m_bufferChart->legend()->hide();
//    m_bufferChart->createDefaultAxes();
    m_bufferChart->addSeries(m_bufferSeries);
    m_bufferChart->setTitle("buffer");

    QChartView *graphChartViewDerivative = new QChartView(m_graphChartDerivative);
    QChartView *graphChartViewDiscrepancy = new QChartView(m_graphChartDiscrepancy);
    QChartView *graphChartViewPreviousDerivative = new QChartView(m_graphChartPreviousDerivative);

    m_bufferChartView = new QChartView(m_bufferChart);
    //graphChartViewDerivative->setRenderHint(QPainter::Antialiasing);

    //ui->horizontalLayout->addWidget(graphChartViewDerivative);
    ui->gridLayout->addWidget(graphChartViewDiscrepancy,1,1);
    ui->gridLayout->addWidget(graphChartViewDerivative,2,1);
    ui->gridLayout->addWidget(graphChartViewPreviousDerivative,3,1);
    ui->horizontalLayout_2->addWidget(m_bufferChartView);


    m_videoProbe = new QVideoProbe(this);
    connect(m_videoProbe, &QVideoProbe::videoFrameProbed, m_fpsWidget, &FPSWidget::processFrame);
    connect(m_fpsWidget, SIGNAL(updatelineedit(int)), this, SLOT(updatelineedit(int)));
    m_videoProbe->setSource(m_player);

    connect(m_fpsWidget, SIGNAL(frameReady(QVector<double>, QVector<double>, QVector<double>, int)), this, SLOT(processChart(QVector<double>, QVector<double>, QVector<double>, int)));

    connect(this, SIGNAL(zoneChanged(int,int)), m_fpsWidget,SLOT(setZone(int,int)));


    ui->textBrowser->document()->setMaximumBlockCount(75);


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

void NewPlayer::processChart(QVector<double> graphDerivative, QVector<double> graphDiscrepancy,QVector<double> graphPreviousDerivative, int shift)
{
    ui->label->setText(QString::number(shift));

    shiftBuffer(shift);

    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();
    QLineSeries *series3 = new QLineSeries();

    for(int counter = 0; counter < graphDerivative.size(); counter++)
    {
        series1->append(counter,graphDerivative[counter]);
        series3->append(counter,graphPreviousDerivative[counter]);
    }
    for(int counter = 0; counter < graphDiscrepancy.size(); counter++)
    {

        graphDiscrepancy[counter] = qLn(graphDiscrepancy[counter]);//из-за этого тоже выскакивает nan, и это тоже нужно)


        series2->append(counter,graphDiscrepancy[counter]);
    }

    m_graphChartDerivative->removeAllSeries();
    m_graphChartDiscrepancy->removeAllSeries();
    m_graphChartPreviousDerivative->removeAllSeries();


    m_graphChartDerivative->addSeries(series1);
    m_graphChartDiscrepancy->addSeries(series2);
    m_graphChartPreviousDerivative->addSeries(series3);

    m_graphChartDerivative->createDefaultAxes();
    m_graphChartDiscrepancy->createDefaultAxes();
    m_graphChartPreviousDerivative->createDefaultAxes();

}


void NewPlayer::shiftBuffer(int shift)
{
//    if(m_bufferCounter%2)
//        shift = 50;
//    else
//        shift = -50;
    ui->textBrowser->append(QString::number(shift));


    if(!(m_bufferCounter%200))
    {
        QValueAxis *axisY = new QValueAxis();

        if(m_bufferCounter == 0)
        {
            axisY->setRange(-500,500);
            m_bufferChart->addAxis(axisY, Qt::AlignLeft);
            m_bufferSeries->attachAxis(axisY);

        }


        QValueAxis *axisX = new QValueAxis();
        if(m_bufferCounter == 0)
        {
            axisX->setRange(0,200);
            m_bufferChart->addAxis(axisX, Qt::AlignBottom);
            m_bufferSeries->attachAxis(axisX);
        }
        else
        {
            axisX->setVisible(false);
//            m_bufferChart->removeAxis(axisX);
            axisX->setRange(m_bufferCounter, m_bufferCounter+200);
            axisX->setMax(m_bufferCounter+200);
            m_bufferChart->addAxis(axisX, Qt::AlignBottom);
            m_bufferSeries->attachAxis(axisX);
        }

//        m_bufferSeries->attachAxis(axisX);
//        m_bufferSeries->attachAxis(axisY);
    }


//    int i = m_bufferCounter%200;



    m_bufferSeries->append(m_bufferCounter, shift);
//    int g;
//    if(m_bufferCounter%2)
//        g = 1;
//    else
//        g = 2;

//    m_bufferSeries->append(m_bufferCounter, g);

//    m_bufferChart->removeAllSeries();




    m_bufferCounter++;





//    m_bufferChart->removeAllSeries();
//    m_bufferChart->addSeries(m_bufferSeries);
//    m_bufferChart->createDefaultAxes();
}



void NewPlayer::on_editZone_clicked()
{
    int width = ui->spinBox->value();
    int height = ui->spinBox_2->value();




    emit zoneChanged(width,height);

}

