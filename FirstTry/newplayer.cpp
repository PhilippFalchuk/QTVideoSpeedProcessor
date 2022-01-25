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
    m_bufferSeriesColor = new QLineSeries();
//    m_bufferSeries->append(0, 0);
//    m_bufferSeries->append(2, 2);

    m_seriesDerivative = new QLineSeries();
    m_seriesDiscrepancy = new QLineSeries();
    m_seriesPreviousDerivative = new QLineSeries();


    m_graphChartDerivative = new QChart();
    m_graphChartDerivative->legend()->hide();
    m_graphChartDerivative->createDefaultAxes();
    //m_graphChartDerivative->addSeries(m_series);
    m_graphChartDerivative->setTitle("derivative");

    m_graphChartDiscrepancy = new QChart();
    m_graphChartDiscrepancy->legend()->hide();
    m_graphChartDiscrepancy->createDefaultAxes();
    m_graphChartDiscrepancy->setTitle("discrepancy");

    m_graphChartPreviousDerivative =new QChart();
    m_graphChartPreviousDerivative->legend()->hide();
    m_graphChartPreviousDerivative->createDefaultAxes();
    m_graphChartPreviousDerivative->setTitle("derivative2");

    m_graphChartColor = new QChart();
    m_graphChartColor->legend()->hide();
    m_graphChartColor->createDefaultAxes();
    m_graphChartColor->setTitle("Color");

    m_graphChartDiscrepancyColor = new QChart();
    m_graphChartDiscrepancyColor->legend()->hide();
    m_graphChartDiscrepancyColor->createDefaultAxes();
    m_graphChartDiscrepancyColor->setTitle("discrepancy Color");

    m_graphChartPreviousColor = new QChart();
    m_graphChartPreviousColor->legend()->hide();
    m_graphChartPreviousColor->createDefaultAxes();
    m_graphChartPreviousColor->setTitle("Color2");

    m_bufferChart =new QChart();
    m_bufferChart->legend()->hide();
//    m_bufferChart->createDefaultAxes();
    m_bufferChart->addSeries(m_bufferSeries);
    m_bufferChart->addSeries(m_bufferSeriesColor);
    m_bufferChart->setTitle("buffer");


    m_graphChartDerivative->addSeries(m_seriesDerivative);
    m_graphChartDiscrepancy->addSeries(m_seriesDiscrepancy);
    m_graphChartPreviousDerivative->addSeries(m_seriesPreviousDerivative);

    m_axisDerX = new QValueAxis();
    m_axisDerX->setRange(0,850);
    m_graphChartDerivative->addAxis(m_axisDerX, Qt::AlignBottom);
    m_seriesDerivative->attachAxis(m_axisDerX);

    QValueAxis* axisDerY = new QValueAxis();
    axisDerY->setRange(-20,20);
    m_graphChartDerivative->addAxis(axisDerY, Qt::AlignLeft);
    m_seriesDerivative->attachAxis(axisDerY);


    m_axisPrevDerX = new QValueAxis();
    m_axisPrevDerX->setRange(0,850);
    m_graphChartPreviousDerivative->addAxis(m_axisPrevDerX, Qt::AlignBottom);
    m_seriesPreviousDerivative->attachAxis(m_axisPrevDerX);

    QValueAxis* axisPrevDerY = new QValueAxis();
    axisPrevDerY->setRange(-20,20);
    m_graphChartPreviousDerivative->addAxis(axisPrevDerY, Qt::AlignLeft);
    m_seriesPreviousDerivative->attachAxis(axisPrevDerY);

    m_axisDisX = new QValueAxis();
    m_axisDisX->setRange(0,1700);
    m_graphChartDiscrepancy->addAxis(m_axisDisX, Qt::AlignBottom);
    m_seriesDiscrepancy->attachAxis(m_axisDisX);

    QValueAxis* axisDisY = new QValueAxis();
    axisDisY->setRange(-20,5);
    m_graphChartDiscrepancy->addAxis(axisDisY, Qt::AlignLeft);
    m_seriesDiscrepancy->attachAxis(axisDisY);


    QChartView *graphChartViewDerivative = new QChartView(m_graphChartDerivative);
    QChartView *graphChartViewDiscrepancy = new QChartView(m_graphChartDiscrepancy);
    QChartView *graphChartViewPreviousDerivative = new QChartView(m_graphChartPreviousDerivative);

    QChartView *graphChartViewColor = new QChartView(m_graphChartColor);
    QChartView *graphChartViewDiscrepancyColor = new QChartView(m_graphChartDiscrepancyColor);
    QChartView *graphChartViewPreviousColor = new QChartView(m_graphChartPreviousColor);

    m_bufferChartView = new QChartView(m_bufferChart);
    //graphChartViewDerivative->setRenderHint(QPainter::Antialiasing);

    //ui->horizontalLayout->addWidget(graphChartViewDerivative);
    ui->gridLayout->addWidget(graphChartViewDiscrepancy,1,1);
    ui->gridLayout->addWidget(graphChartViewDerivative,2,1);
    ui->gridLayout->addWidget(graphChartViewPreviousDerivative,3,1);
    ui->horizontalLayout_2->addWidget(m_bufferChartView);

    ui->gridLayout_2->addWidget(graphChartViewDiscrepancyColor,1,1);
    ui->gridLayout_2->addWidget(graphChartViewColor,2,1);
    ui->gridLayout_2->addWidget(graphChartViewPreviousColor,3,1);


    m_videoProbe = new QVideoProbe(this);
    connect(m_videoProbe, &QVideoProbe::videoFrameProbed, m_fpsWidget, &FPSWidget::processFrame);
    connect(m_fpsWidget, SIGNAL(updatelineedit(int)), this, SLOT(updatelineedit(int)));
    m_videoProbe->setSource(m_player);

    connect(m_fpsWidget, SIGNAL(frameReady(QVector<double>, QVector<double>, QVector<double>, int, int)), this, SLOT(processChart(QVector<double>, QVector<double>, QVector<double>, int, int)));
    connect(m_fpsWidget, SIGNAL(frameReadyColor(QVector<double>, QVector<double>, QVector<double>, int)), this, SLOT(processChartColor(QVector<double>, QVector<double>, QVector<double>, int)));
    connect(m_fpsWidget, SIGNAL(maskReady(QImage)), this, SLOT(displayMask(QImage)));

    connect(this, SIGNAL(zoneChanged(int,int)), m_fpsWidget,SLOT(setZone(int,int)));



    ui->textBrowser->document()->setMaximumBlockCount(75);

    ui->imageLabel->setBackgroundRole(QPalette::Dark);
    ui->imageLabel->setScaledContents(true);

}

NewPlayer::~NewPlayer()
{
    delete ui;
}


void NewPlayer::on_pushButton_clicked()
{
    m_player->setMedia(QUrl("rtsp://user:h5106120@192.168.0.228:554/ISAPI/Streaming/Channels/101"));
    m_player->play();
//    QPushButton *btn = new QPushButton(m_videoWidget);
//    btn->setText("sdfasfasdfd\n\n\n\n\n\n\nfsadfasdfasffda");
//    btn->setVisible(true);
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

void NewPlayer::processChart(QVector<double> graphDerivative, QVector<double> graphDiscrepancy,QVector<double> graphPreviousDerivative, int shift, int framesCount)
{
    ui->label->setText(QString::number(shift));
    shiftBuffer(shift, framesCount);


//    m_seriesDerivative->clear();
//    m_seriesDiscrepancy->clear();
//    m_seriesPreviousDerivative->clear();

    QVector<QPointF> pointDerivativeVector(graphDerivative.size());
    QVector<QPointF> pointPreviousDerivativeVector(graphPreviousDerivative.size());
    QVector<QPointF> pointDiscrepancyVector(graphDiscrepancy.size());



    for(int counter = 0; counter < graphDerivative.size(); counter++)
    {
        //m_seriesDerivative->append(counter,graphDerivative[counter]);
        //m_seriesPreviousDerivative->append(counter,graphPreviousDerivative[counter]);

        pointDerivativeVector[counter] = QPointF(counter, graphDerivative[counter]);
        pointPreviousDerivativeVector[counter] = QPointF(counter, graphPreviousDerivative[counter]);

    }
    for(int counter = 0; counter < graphDiscrepancy.size(); counter++)
    {

//        graphDiscrepancy[counter] = qLn(graphDiscrepancy[counter]);//из-за этого тоже выскакивает nan, и это тоже нужно)
//        m_seriesDiscrepancy->append(counter,graphDiscrepancy[counter]);

        graphDiscrepancy[counter] = qLn(graphDiscrepancy[counter]);

        pointDiscrepancyVector[counter] = QPointF(counter, graphDiscrepancy[counter]);

    }

    //m_graphChartDerivative->removeAllSeries();
    //m_graphChartDiscrepancy->removeAllSeries();
    //m_graphChartPreviousDerivative->removeAllSeries();



    m_seriesDerivative->replace(pointDerivativeVector);
    m_seriesPreviousDerivative->replace(pointPreviousDerivativeVector);
    m_seriesDiscrepancy->replace(pointDiscrepancyVector);



}


void NewPlayer::shiftBuffer(int shift, int framesCount)
{
//    if(m_bufferCounter%2)
//        shift = 50;
//    else
//        shift = -50;
    ui->textBrowser->append(QString::number(shift) + "   " + QString::number(framesCount));



    if(m_bufferCounter == 0)
    {
        m_axisY = new QValueAxis();
        m_axisY->setRange(-500,500);
        m_bufferChart->addAxis(m_axisY, Qt::AlignLeft);
        m_bufferSeries->attachAxis(m_axisY);

        m_axisX = new QValueAxis();
        m_axisX->setRange(0,400);
        m_bufferChart->addAxis(m_axisX, Qt::AlignBottom);
        m_bufferSeries->attachAxis(m_axisX);
    }


//    int i = m_bufferCounter%200;

    if(!(m_bufferCounter%400))
        m_bufferSeries->clear();

    m_bufferSeries->append(m_bufferCounter%400, shift);

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

    m_axisDerX->setRange(0, width -1);
    m_axisPrevDerX->setRange(0, width -1);
    m_axisDisX->setRange(0, width*2 -2);


    emit zoneChanged(width,height);

}

void NewPlayer::processChartColor(QVector<double> graphDerivative, QVector<double> graphDiscrepancy,QVector<double> graphPreviousDerivative, int shift)
{


    shiftBufferColor(shift);

//    QLineSeries *series1 = new QLineSeries();
//    QLineSeries *series2 = new QLineSeries();
//    QLineSeries *series3 = new QLineSeries();

//    for(int counter = 0; counter < graphDerivative.size(); counter++)
//    {
//        series1->append(counter,graphDerivative[counter]);
//        series3->append(counter,graphPreviousDerivative[counter]);
//    }
//    for(int counter = 0; counter < graphDiscrepancy.size(); counter++)
//    {

//        graphDiscrepancy[counter] = qLn(graphDiscrepancy[counter]);//из-за этого тоже выскакивает nan, и это тоже нужно)


//        series2->append(counter,graphDiscrepancy[counter]);
//    }

//    m_graphChartColor->removeAllSeries();
//    m_graphChartDiscrepancyColor->removeAllSeries();
//    m_graphChartPreviousColor->removeAllSeries();


//    m_graphChartColor->addSeries(series1);
//    m_graphChartDiscrepancyColor->addSeries(series2);
//    m_graphChartPreviousColor->addSeries(series3);

//    m_graphChartColor->createDefaultAxes();
//    m_graphChartDiscrepancyColor->createDefaultAxes();
//    m_graphChartPreviousColor->createDefaultAxes();

}

void NewPlayer::shiftBufferColor(int shift)
{





    if(m_bufferCounterColor == 1)
    {
        m_bufferSeriesColor->attachAxis(m_axisX);
        m_bufferSeriesColor->attachAxis(m_axisY);
    }




    if(!(m_bufferCounterColor%400))
        m_bufferSeriesColor->clear();

    m_bufferSeriesColor->append(m_bufferCounterColor%400, shift);






    m_bufferCounterColor++;





//    m_bufferChart->removeAllSeries();
//    m_bufferChart->addSeries(m_bufferSeries);
//    m_bufferChart->createDefaultAxes();
}

void NewPlayer::displayMask(QImage maskImage)
{
    ui->imageLabel->setPixmap(QPixmap::fromImage(maskImage.mirrored(false,true)));
//    qDebug << outputPixmap;
}

