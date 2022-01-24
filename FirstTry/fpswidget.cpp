#include "fpswidget.h"
#include <QTimer>
#include <QPainter>
#include <QRandomGenerator>

FPSWidget::FPSWidget(QLabel *parent)
    : QLabel(parent)
{
    m_backgroundColor = Qt::white;
    m_margin = 4;
    m_fpsCounter = 0;

    m_processor.moveToThread(&m_processorThread);
    qRegisterMetaType<QVector<double>>("QVector<double>");
    //connect(&m_processor, &FrameProcessor::frameProcessed, this, &FPSWidget::frameReady);
    connect(&m_processor, SIGNAL(frameProcessed(QVector<double>, QVector<double>,QVector<double>, int)), this, SIGNAL(frameReady(QVector<double>, QVector<double>, QVector<double>, int)));
    m_processorThread.start(QThread::LowestPriority);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FPSWidget::refreshCounter);
    timer->start(1000);

}

FPSWidget::~FPSWidget()
{
    m_processorThread.quit();
    m_processorThread.wait(1000);
}

void FPSWidget::processFrame(const QVideoFrame &frame)
{
    m_fpsCounter++;

    QMetaObject::invokeMethod(&m_processor, "processFrame",
                              Qt::QueuedConnection, Q_ARG(QVideoFrame, frame), Q_ARG(int, m_zoneWidth), Q_ARG(int, m_zoneHeight));
}

void FPSWidget::refreshCounter()
{

    emit updatelineedit(m_fpsCounter);
    QString str = QString(m_fpsCounter);
    this->setText(str);
    this->repaint();


    m_fpsCounter = 0;
}

void FPSWidget::setZone(int width, int height)
{
    m_zoneWidth = width;
    m_zoneHeight = height;
}


void FPSWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();

    painter.setBrush(m_backgroundColor);
    painter.drawRect(rect());

    painter.restore();

    QString str = QString::number(m_fpsCounter);
    painter.drawText(rect(), Qt::AlignCenter, str);

}

QTime fromMSec( quint64 totalMsec )
{
    uint hour = totalMsec / 1000 / 60 / 60 % 60;
    uint minute = totalMsec / 1000 / 60 % 60;
    uint sec = totalMsec / 1000 % 60;
    uint msec = totalMsec % 1000;

    return QTime( hour, minute, sec, msec );
}

void FrameProcessor::processFrame(QVideoFrame frame, int zoneWidth, int zoneHeight)
{
//    QTime t1, t3;
//    quint64 msec;
//    t1 = QTime::currentTime();

    counterframe++;

    int widthOfImage = 1280;
    int heightOfImage = 720;

    int widthOfZone;
    int heightOfZone;
    int startOfZoneWidth;
    int startOfZoneHeight;
    if(zoneWidth ==0 || zoneHeight == 0)
    {
        widthOfZone = (widthOfImage*2)/3;
        heightOfZone = (heightOfImage*2)/3;
        startOfZoneWidth = (widthOfImage - widthOfZone)/2;
        startOfZoneHeight = (heightOfImage - heightOfZone)/2;
    }
    else
    {
        widthOfZone = zoneWidth;
        heightOfZone = zoneHeight;
        startOfZoneWidth = (widthOfImage - widthOfZone)/2;
        startOfZoneHeight = (heightOfImage - heightOfZone)/2;
    }
    QVector<double> graphBWA(widthOfZone);
    QVector<double> graphDerivative(graphBWA.size() - 1);
    QVector<double> graphDiscrepancy(graphDerivative.size()*2);
    QVector<double> cutDiscrepancy(graphDerivative.size());
    m_previousGraphDerivative.resize(graphDerivative.size());
    int indOfMinCut;
    int shiftOfDis;
    do
    {
        if(!frame.map(QAbstractVideoBuffer::ReadOnly))
            break;

        QImage::Format imageFormat =QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
        //qDebug()<<imageFormat;
        if(imageFormat != QImage::Format_Invalid)
        {
            QImage image/* = frame.image();*/(frame.bits(), frame.width(), frame.height(), imageFormat);
            image = image.convertToFormat(QImage::Format_RGB32);


            for (int y = startOfZoneHeight; y < image.height() - startOfZoneHeight; ++y)
            {
             for(int x = startOfZoneWidth; x < image.width() - startOfZoneWidth -1; x++)
             {
                 graphBWA[x - startOfZoneWidth] += qGray(image.pixel(x,y))/(static_cast<double>(heightOfZone));
             }

            }



            int sizeBWA = graphBWA.size();
            for(int counter = 0; counter < sizeBWA - 1; counter++)
            {
                double delta = graphBWA[counter + 1] - graphBWA[counter];




//                if(delta < 0)
//                    graphDerivative[counter] = -(delta);
//                else
                graphDerivative[counter] = delta;
//                graphDerivative[counter] = 0;
//                if (counterframe%2 == 0 && counter == 100) graphDerivative[counter] = 120;
//                if (counterframe%2 == 0 && counter == 220) graphDerivative[counter] = 44;
//                if (counterframe%2 == 0 && counter == 240) graphDerivative[counter] = 44;
//                if (counterframe%2 == 0 && counter == 280) graphDerivative[counter] = 44;
//                if (counterframe%2 == 0 && counter == 300) graphDerivative[counter] = 44;
//                if (counterframe%2 == 0 && counter == 400) graphDerivative[counter] = 44;

//                if (counterframe%2 == 0 && counter == 520) graphDerivative[counter] = 46;
//                if (counterframe%2 == 0 && counter == 540) graphDerivative[counter] = 46;
//                if (counterframe%2 == 0 && counter == 580) graphDerivative[counter] = 46;
//                if (counterframe%2 == 0 && counter == 600) graphDerivative[counter] = 46;
//                if (counterframe%2 == 0 && counter == 700) graphDerivative[counter] = 46;

//                if (counterframe%2 != 0 && counter == 101) graphDerivative[counter] = 120;
//                if (counterframe%2 != 0 && counter == 230) graphDerivative[counter] = 44;
//                if (counterframe%2 != 0 && counter == 250) graphDerivative[counter] = 44;
//                if (counterframe%2 != 0 && counter == 290) graphDerivative[counter] = 44;
//                if (counterframe%2 != 0 && counter == 310) graphDerivative[counter] = 44;
//                if (counterframe%2 != 0 && counter == 410) graphDerivative[counter] = 44;

//                if (counterframe%2 != 0 && counter == 525) graphDerivative[counter] = 46;
//                if (counterframe%2 != 0 && counter == 545) graphDerivative[counter] = 46;
//                if (counterframe%2 != 0 && counter == 585) graphDerivative[counter] = 46;
//                if (counterframe%2 != 0 && counter == 605) graphDerivative[counter] = 46;
//                if (counterframe%2 != 0 && counter == 705) graphDerivative[counter] = 46;

            }

//            for(int counter = 0; counter < graphBWA.size() - 1;counter++)
//            {
//                graphDerivative[counter] = pow(2.71, -(counter-450)*(counter-450)/(2500));
//                //graphDerivative[counter] = counter%((graphBWA.size()-1)/2);
//                m_previousGraphDerivative[counter] = pow(2.71, -(counter-500)*(counter-500)/(2500));
//            }


            if(!m_previousGraphDerivative.isEmpty())
            {
                for(int shift = -(graphDerivative.size()); shift < graphDerivative.size(); shift++)
                {
                    double int1=0,int2=0;
                    if(shift<0)
                    {
                        for(int i = 0 ; i < graphDerivative.size(); i++)
                        {
                            if((i+shift)>0 && (i+shift)<graphDerivative.size())
                            {
                                graphDiscrepancy[shift+graphDerivative.size()] += (m_previousGraphDerivative[i] - graphDerivative[i + shift])  *  (m_previousGraphDerivative[i] - graphDerivative[i + shift]);
                                int1 += m_previousGraphDerivative[i]*m_previousGraphDerivative[i];
                                int2 += graphDerivative[i + shift]*graphDerivative[i + shift];
                            }
                        }
                    }
                    else
                    {
                        for(int i = 0; i < graphDerivative.size(); i++)
                        {
                            if((i+shift)>0 && (i+shift)<graphDerivative.size())
                            {
                                graphDiscrepancy[shift + graphDerivative.size()] += (m_previousGraphDerivative[i] - graphDerivative[i + shift])*(m_previousGraphDerivative[i] - graphDerivative[i + shift]);
                                int1 += m_previousGraphDerivative[i]*m_previousGraphDerivative[i];
                                int2 += graphDerivative[i + shift]*graphDerivative[i + shift];
                            }
                        }
                    }
                    //нормируем на интегралы
                    if(int1*int2>0)
                        graphDiscrepancy[shift+graphDerivative.size()] /= (int1*int2);
                    else
                        graphDiscrepancy[shift+graphDerivative.size()] = NAN;//из-за этого в консоли выскакивают nan, но это убирает завалы невязки))


                }
            }


            int startOfCut = graphDiscrepancy.size()/4;
            int endOfCut = (graphDiscrepancy.size()*3)/4;
            if(!graphDiscrepancy.isEmpty())
            {
                for(int i = startOfCut ; i < endOfCut; i++)
                {
                    cutDiscrepancy[i - startOfCut] = graphDiscrepancy[i];
                }

                qreal minValueOfCut = __DBL_MAX__;
                for(int counter = 0; counter < cutDiscrepancy.size(); counter++)
                {
                    if(cutDiscrepancy[counter] < minValueOfCut)
                    {
                        minValueOfCut = cutDiscrepancy[counter];
                        indOfMinCut = counter;
                    }
                }

                int mainX = startOfCut + indOfMinCut;
                shiftOfDis = mainX - graphDerivative.size();
            }

            //qDebug() << graphDiscrepancy;


        }
    }while(false);

//    msec = qAbs(QTime::currentTime().msecsTo(t1));
//    t3 = fromMSec(msec);

//    qDebug()<<t3;


    emit frameProcessed(graphDerivative, graphDiscrepancy, m_previousGraphDerivative, shiftOfDis);
    m_previousGraphDerivative = graphDerivative;
}

FrameProcessor::FrameProcessor(QObject *parent)
    : QObject(parent)
{
//   m_previousGraphDerivative.resize(852);
}
