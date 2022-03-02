#include "threadhandler.h"
#include <QTimer>
#include <QPainter>
#include <QRandomGenerator>

ThreadHandler::ThreadHandler(QLabel *parent)
    : QLabel(parent)
{
    m_backgroundColor = Qt::white;
    m_margin = 4;
    m_fpsCounter = 0;

    m_processor.moveToThread(&m_processorThread);
    qRegisterMetaType<QVector<double>>("QVector<double>");
    //connect(&m_processor, &FrameProcessor::frameProcessed, this, &FPSWidget::frameReady);
    connect(&m_processor, SIGNAL(frameProcessed(QVector<double>,QVector<double>,QVector<double>, int, int)), this, SIGNAL(frameReady(QVector<double>,QVector<double>,QVector<double>, int, int)));
    m_processorThread.start(QThread::LowestPriority);
}

ThreadHandler::~ThreadHandler()
{
    m_processorThread.quit();
    m_processorThread.wait(1000);
}

void ThreadHandler::processFrame(const QVideoFrame &frame)
{
    m_fpsCounter++;

    QMetaObject::invokeMethod(&m_processor, "processFrame",
                              Qt::QueuedConnection, Q_ARG(QVideoFrame, frame), Q_ARG(int, m_zoneWidth), Q_ARG(int, m_zoneHeight), Q_ARG(int, m_widthOfDis), Q_ARG(bool, m_saverFlag));
}

void ThreadHandler::refreshCounter()
{
    emit updatelineedit(m_fpsCounter);
    m_fpsCounter = 0;
}

void ThreadHandler::setZone(int width, int height, int widthOfDis,bool saverFlag)
{
    m_zoneWidth = width;
    m_zoneHeight = height;
    m_widthOfDis = widthOfDis;
    m_saverFlag = saverFlag;
}

void FrameProcessor::processFrame(QVideoFrame frame, int zoneWidth, int zoneHeight, int widthOfDis, bool saverFlag)
{
    m_counterFrame++;

    int widthOfImage = 1280;
    int heightOfImage = 720;
    int centerOfImageWidth = widthOfImage/2;

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

        if(!(widthOfZone%2))
            widthOfZone -= 1;
    }

    QVector<double> graphBWA(widthOfZone);
    QVector<double> graphDerivative(graphBWA.size() - 1);
    QVector<double> graphDiscrepancy(graphDerivative.size()*2);
    QVector<double> cutDiscrepancy(graphDerivative.size());

    m_previousGraphDerivative.resize(graphDerivative.size());
    int indOfMinDis;
    int shiftOfDis;
    int minShiftOfCalc;
    int maxShiftOfCalc;
    if(!widthOfDis)
    {
        minShiftOfCalc = -(widthOfZone/2);
        maxShiftOfCalc = (widthOfZone/2);
    }
    else
    {
        minShiftOfCalc = -(widthOfDis/2);
        maxShiftOfCalc = (widthOfDis/2);
    }

    do
    {
        if(!frame.map(QAbstractVideoBuffer::ReadOnly))
            break;

        qDebug() << frame.pixelFormat();
        QImage::Format imageFormat =QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
        if(imageFormat != QImage::Format_Invalid)
        {
            QImage image(frame.bits(), frame.width(), frame.height(), imageFormat);

            for(int y = startOfZoneHeight; y < heightOfImage - startOfZoneHeight - 1; y ++)
            {
                uchar* scan = image.scanLine(y);
                int depth =4;
                for(int x = startOfZoneWidth; x < widthOfImage - startOfZoneWidth - 1; x++)
                {
                     graphBWA[x-startOfZoneWidth]+= qGray(*(reinterpret_cast<QRgb*>(scan + x*depth)));
                }
            }

            for(int i = 0; i < graphBWA.size(); i ++)
            {
                graphBWA[i] /= static_cast<double>(heightOfZone);
            }

            int sizeBWA = graphBWA.size();
            for(int counter = 0; counter < sizeBWA - 1; counter++)
            {
                double delta = graphBWA[counter + 1] - graphBWA[counter];
                graphDerivative[counter] = delta;
            }

            if(!m_previousGraphDerivative.isEmpty()/* && !m_previousGraphBWA.isEmpty()*/)
            {
                for(int shift = minShiftOfCalc; shift < maxShiftOfCalc; shift++)
                {
                        double int1=0,int2=0;//int1b = 0, int2b = 0;
                        if(shift<0)
                        {
                            for(int i = 0 ; i < graphDerivative.size()+shift; i++)
                            {
                                graphDiscrepancy[shift+graphDerivative.size()] += (m_previousGraphDerivative[i-shift] - graphDerivative[i])  *  (m_previousGraphDerivative[i-shift] - graphDerivative[i]);
                                int1 += m_previousGraphDerivative[i-shift]*m_previousGraphDerivative[i-shift];
                                int2 += graphDerivative[i]*graphDerivative[i];
                            }
                        }
                        else
                        {
                            for(int i = shift; i < graphDerivative.size(); i++)
                            {
                                graphDiscrepancy[shift + graphDerivative.size()] += (m_previousGraphDerivative[i-shift] - graphDerivative[i])*(m_previousGraphDerivative[i-shift] - graphDerivative[i]);
                                int1 += m_previousGraphDerivative[i-shift]*m_previousGraphDerivative[i-shift];
                                int2 += graphDerivative[i]*graphDerivative[i];
                            }
                        }
                        //нормируем на интегралы
                        if(int1*int2>0)
                            graphDiscrepancy[shift+graphDerivative.size()] /= (int1*int2);
                        else
                            graphDiscrepancy[shift+graphDerivative.size()] = NAN;//из-за этого в консоли выскакивают nan, но это убирает завалы невязки))

                }
            }

            if(!graphDiscrepancy.isEmpty())
            {
                qreal minValueOfDis = __DBL_MAX__;
                for(int i = minShiftOfCalc + graphDerivative.size()+1; i < maxShiftOfCalc + graphDerivative.size() -1; i++)
                {
                    if(graphDiscrepancy[i] < minValueOfDis)
                    {
                        minValueOfDis = graphDiscrepancy[i];
                        indOfMinDis = i;
                    }
                }

                shiftOfDis = indOfMinDis - graphDerivative.size();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if(m_counterFrame > 4 && saverFlag)
                {
                    if(shiftOfDis < -3)
                    {
                        QImage cropped;
                        cropped = image.copy(centerOfImageWidth + shiftOfDis, 0, -shiftOfDis, 720).mirrored(true,true);
                        emit imageReady(cropped);
                    }
                    if(shiftOfDis > 3)
                    {
                        QImage cropped;
                        cropped = image.copy(centerOfImageWidth, 0, shiftOfDis, 720).mirrored(false,true);
                        emit imageReady(cropped);
                    }
                }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
        }
    }while(false);

    m_previousGraphDerivative = graphDerivative;
    emit shiftReady(shiftOfDis);
}

FrameProcessor::FrameProcessor(QObject *parent)
    : QObject(parent)
{
    m_mergedImageVector.resize(200);
}




