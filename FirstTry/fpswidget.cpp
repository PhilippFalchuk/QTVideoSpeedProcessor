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
                              Qt::QueuedConnection, Q_ARG(QVideoFrame, frame));
}

void FPSWidget::refreshCounter()
{

    emit updatelineedit(m_fpsCounter);
    QString str = QString(m_fpsCounter);
    this->setText(str);
    this->repaint();


    m_fpsCounter = 0;
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

void FrameProcessor::processFrame(QVideoFrame frame)
{
//    QTime t1, t3;
//    quint64 msec;
//    t1 = QTime::currentTime();


    int widthOfImage = 1280;
    int widthOfZone = (widthOfImage*2)/3;
    int heightOfImage = 720;
    int heightOfZone = (heightOfImage*2)/3;
    int startOfZoneWidth = (widthOfImage - widthOfZone)/2;
    int startOfZoneHeight = (heightOfImage - heightOfZone)/2;
    QVector<double> graphBWA(widthOfZone);
    QVector<double> graphDerivative(graphBWA.size() - 1);
    QVector<double> graphDiscrepancy(graphDerivative.size()*2);
    QVector<double> cutDiscrepancy(graphDerivative.size());
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
             for(int x = startOfZoneWidth; x < image.width() - startOfZoneWidth - 1; x++)
             {
                 graphBWA[x - startOfZoneWidth] += qGray(image.pixel(x,y))/420.;
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
                    if(shift<0)
                    {
                        for(int i = 0 ; i < graphDerivative.size(); i++)
                        {
                            if((i+shift)>0 && (i+shift)<graphDerivative.size())
                                graphDiscrepancy[shift+graphDerivative.size()] += (m_previousGraphDerivative[i] - graphDerivative[i + shift])  *  (m_previousGraphDerivative[i] - graphDerivative[i + shift]);
                        }
                    }
                    else
                    {
                        for(int i = 0; i < graphDerivative.size(); i++)
                        {
                            if((i+shift)>0 && (i+shift)<graphDerivative.size())
                                graphDiscrepancy[shift + graphDerivative.size()] += (m_previousGraphDerivative[i] - graphDerivative[i + shift])*(m_previousGraphDerivative[i] - graphDerivative[i + shift]);
                        }
                    }

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

                int minValueOfCut = 1000;
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
    m_previousGraphDerivative.resize(852);
}
