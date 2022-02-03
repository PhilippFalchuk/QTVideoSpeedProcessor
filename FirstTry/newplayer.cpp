#include "newplayer.h"
#include "ui_newplayer.h"
#include "videowidget.h"
#include <QtMath>
#include <QSerialPortInfo>
#include <QFile>

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
    m_videoWidget->setVisible(true);

    m_threadHandler = new ThreadHandler();
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

    m_seriesColor = new QLineSeries();
    m_seriesDiscrepancyColor = new QLineSeries();
    m_seriesPreviousColor = new QLineSeries();


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

    m_graphChartColor->addSeries(m_seriesColor);
    m_graphChartDiscrepancyColor->addSeries(m_seriesDiscrepancyColor);
    m_graphChartPreviousColor->addSeries(m_seriesPreviousColor);

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
    m_axisDisX->setRange(425,1275);
    m_graphChartDiscrepancy->addAxis(m_axisDisX, Qt::AlignBottom);
    m_seriesDiscrepancy->attachAxis(m_axisDisX);

    QValueAxis* axisDisY = new QValueAxis();
    axisDisY->setRange(-20,5);
    m_graphChartDiscrepancy->addAxis(axisDisY, Qt::AlignLeft);
    m_seriesDiscrepancy->attachAxis(axisDisY);

    m_axisColorX = new QValueAxis();
    m_axisColorX->setRange(0,850);
    m_graphChartColor->addAxis(m_axisColorX, Qt::AlignBottom);
    m_seriesColor->attachAxis(m_axisColorX);

    QValueAxis* axisColorY = new QValueAxis();
    axisColorY->setRange(0,220);
    m_graphChartColor->addAxis(axisColorY, Qt::AlignLeft);
    m_seriesColor->attachAxis(axisColorY);

    m_axisPrevColorX = new QValueAxis();
    m_axisPrevColorX->setRange(0,850);
    m_graphChartPreviousColor->addAxis(m_axisPrevColorX, Qt::AlignBottom);
    m_seriesPreviousColor->attachAxis(m_axisPrevColorX);

    QValueAxis* axisPrevColorY = new QValueAxis();
    axisPrevColorY->setRange(0,220);
    m_graphChartPreviousColor->addAxis(axisPrevColorY, Qt::AlignLeft);
    m_seriesPreviousColor->attachAxis(axisPrevColorY);

    m_axisDisColorX = new QValueAxis();
    m_axisDisColorX->setRange(425,1275);
    m_graphChartDiscrepancyColor->addAxis(m_axisDisColorX, Qt::AlignBottom);
    m_seriesDiscrepancyColor->attachAxis(m_axisDisColorX);

    QValueAxis* axisDisColorY = new QValueAxis();
    axisDisColorY->setRange(-10,20);
    m_graphChartDiscrepancyColor->addAxis(axisDisColorY, Qt::AlignLeft);
    m_seriesDiscrepancyColor->attachAxis(axisDisColorY);


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
    connect(m_videoProbe, &QVideoProbe::videoFrameProbed, m_threadHandler, &ThreadHandler::processFrame);
    connect(m_threadHandler, SIGNAL(updatelineedit(int)), this, SLOT(updatelineedit(int)));
    m_videoProbe->setSource(m_player);

    connect(m_threadHandler, SIGNAL(frameReady(QVector<double>,QVector<double>,QVector<double>, int, int)), this, SLOT(processChart(QVector<double>,QVector<double>,QVector<double>, int, int)));
    connect(m_threadHandler, SIGNAL(maskReady(QImage)), this, SLOT(displayMask(QImage)));

    connect(this, SIGNAL(zoneChanged(int,int,int)), m_threadHandler,SLOT(setZone(int,int,int)));



    ui->textBrowser->document()->setMaximumBlockCount(75);

    ui->imageLabel->setBackgroundRole(QPalette::Dark);
    ui->imageLabel->setScaledContents(true);




    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        ui->comboBox->addItem(info.portName());


    connect(this, &NewPlayer::portChanged, &m_comThread, &ComPortThread::changePort);

    connect(&m_threadHandler->m_processor, &FrameProcessor::shiftReady, &m_comThread, &ComPortThread::sendShift);


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

}

NewPlayer::~NewPlayer()
{
    delete ui;
}


void NewPlayer::on_pushButton_clicked()
{
    if(QFile::exists(m_pathToUrl))
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

    QVector<QPointF> pointColorVector(graphDerivative.size());
    QVector<QPointF> pointPreviousColorVector(graphPreviousDerivative.size());
    QVector<QPointF> pointDiscrepancyColorVector(graphDiscrepancy.size());



    for(int counter = 0; counter < graphDerivative.size(); counter++)
    {
        //m_seriesDerivative->append(counter,graphDerivative[counter]);
        //m_seriesPreviousDerivative->append(counter,graphPreviousDerivative[counter]);

        pointDerivativeVector[counter] = QPointF(counter, graphDerivative[counter]);
        pointPreviousDerivativeVector[counter] = QPointF(counter, graphPreviousDerivative[counter]);

        //pointColorVector[counter] = QPointF(counter, graphBWA[counter]);
       // pointPreviousColorVector[counter] = QPointF(counter, graphPreviousBWA[counter]);
    }
    for(int counter = 0; counter < graphDiscrepancy.size(); counter++)
    {

//        graphDiscrepancy[counter] = qLn(graphDiscrepancy[counter]);//из-за этого тоже выскакивает nan, и это тоже нужно)
//        m_seriesDiscrepancy->append(counter,graphDiscrepancy[counter]);

        graphDiscrepancy[counter] = qLn(graphDiscrepancy[counter]);

       //graphDiscrepancyBWA[counter] = qLn(graphDiscrepancyBWA[counter]);

        pointDiscrepancyVector[counter] = QPointF(counter, graphDiscrepancy[counter]);

        //pointDiscrepancyColorVector[counter] = QPointF(counter, graphDiscrepancyBWA[counter]);

    }

    //m_graphChartDerivative->removeAllSeries();
    //m_graphChartDiscrepancy->removeAllSeries();
    //m_graphChartPreviousDerivative->removeAllSeries();



    m_seriesDerivative->replace(pointDerivativeVector);
    m_seriesPreviousDerivative->replace(pointPreviousDerivativeVector);
    m_seriesDiscrepancy->replace(pointDiscrepancyVector);

//    m_seriesColor->replace(pointColorVector);
//    m_seriesPreviousColor->replace(pointPreviousColorVector);
//    m_seriesDiscrepancyColor->replace(pointDiscrepancyColorVector);




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
        m_bufferSeriesColor->attachAxis(m_axisY);

        m_axisX = new QValueAxis();
        m_axisX->setRange(0,400);
        m_bufferChart->addAxis(m_axisX, Qt::AlignBottom);
        m_bufferSeries->attachAxis(m_axisX);
        m_bufferSeriesColor->attachAxis(m_axisX);
    }


//    int i = m_bufferCounter%200;

    if(!(m_bufferCounter%400))
    {
        m_bufferSeries->clear();
        m_bufferSeriesColor->clear();
    }

    m_bufferSeries->append(m_bufferCounter%400, shift);
    //m_bufferSeriesColor->append(m_bufferCounter%400, shiftColor);

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
    int widthOfDis = ui->spinBox_3->value();

//    m_axisDerX->setRange(0, width -1);
//    m_axisPrevDerX->setRange(0, width -1);

//    if(!widthOfDis)
//    {
//        m_axisDisX->setRange((width*2)/4, ((width*2)/4)*3);
//        m_axisDisColorX->setRange((width*2)/4, ((width*2)/4)*3);
//        m_axisY->setRange( -(width*2)/4 , ((width*2)/4));
//    }
//    else
//    {
//        m_axisDisX->setRange(((width*2)-widthOfDis)/2, (width*2) - ((width*2)-widthOfDis)/2);
//        m_axisDisColorX->setRange(((width*2)-widthOfDis)/2, (width*2) - ((width*2)-widthOfDis)/2);
//        m_axisY->setRange( -(widthOfDis/2) , (widthOfDis/2));
//    }




    emit zoneChanged(width,height, widthOfDis);

}





void NewPlayer::displayMask(QImage maskImage)
{
    ui->imageLabel->setPixmap(QPixmap::fromImage(maskImage.mirrored(false,true)));
//    qDebug << outputPixmap;
}


void NewPlayer::on_portPushButton_clicked()
{
    emit portChanged(ui->comboBox->currentText());
}

