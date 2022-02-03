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
    connect(&m_processor, SIGNAL(frameProcessed(QVector<double>, QVector<double>,QVector<double>, int, int, int,QVector<double>,QVector<double>,QVector<double>)), this, SIGNAL(frameReady(QVector<double>, QVector<double>, QVector<double>, int, int, int,QVector<double>,QVector<double>,QVector<double>)));
    m_processorThread.start(QThread::LowestPriority);

    m_maskProcessor.moveToThread(&m_maskThread);
    //qRegisterMetaType<QImage>("QImage");
    connect(&m_maskProcessor, SIGNAL(maskProcessed(QImage)), this, SIGNAL(maskReady(QImage)));
    m_maskThread.start(QThread::LowestPriority);


    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ThreadHandler::refreshCounter);
    timer->start(1000);

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
                              Qt::QueuedConnection, Q_ARG(QVideoFrame, frame), Q_ARG(int, m_zoneWidth), Q_ARG(int, m_zoneHeight), Q_ARG(int, m_widthOfDis));

    QMetaObject::invokeMethod(&m_maskProcessor, "processMask",
                              Qt::QueuedConnection, Q_ARG(QVideoFrame, frame), Q_ARG(int, m_zoneWidth), Q_ARG(int, m_zoneHeight));
}

void ThreadHandler::refreshCounter()
{

    emit updatelineedit(m_fpsCounter);
//    QString str = QString(m_fpsCounter);
//    this->setText(str);
//    this->repaint();


    m_fpsCounter = 0;
}

void ThreadHandler::setZone(int width, int height, int widthOfDis)
{
    m_zoneWidth = width;
    m_zoneHeight = height;
    m_widthOfDis = widthOfDis;
}


void ThreadHandler::paintEvent(QPaintEvent *event)
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

void FrameProcessor::processFrame(QVideoFrame frame, int zoneWidth, int zoneHeight, int widthOfDis)
{
//    QTime t1, t3;
//    quint64 msec;
//    t1 = QTime::currentTime();

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

    QVector<double> graphBWAVertical(heightOfZone);
    QVector<double> graphDerivativeVertical(graphBWAVertical.size() - 1);
    QVector<double> graphDiscrepancyVertical(graphDerivativeVertical.size()*2);

    m_previousGraphDerivative.resize(graphDerivative.size());
    m_previousGraphBWA.resize(graphBWA.size());
    QVector<double> graphDiscrepancyColor(graphBWA.size()*2);
    int indOfMinDis;
    int indOfMinDisColor;
    int shiftOfDis;
    int shiftOfDisColor;

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

        QImage::Format imageFormat =QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
//        qDebug()<<imageFormat;
        if(imageFormat != QImage::Format_Invalid)
        {
            QImage image/* = frame.image();*/(frame.bits(), frame.width(), frame.height(), imageFormat);
            //image = image.convertToFormat(QImage::Format_RGB32);


            for(int x = startOfZoneWidth; x < image.width() - startOfZoneWidth -1; x++)
            {
             for (int y = startOfZoneHeight; y < image.height() - startOfZoneHeight-1; y++)
             {
                 graphBWA[x - startOfZoneWidth] += qGray(image.pixel(x,y));//((image.pixel(x,y)))&0x000000FF;//+((image.pixel(x,y)>>8))&0x000000FF
                         //+((image.pixel(x,y)>>16))&0x000000FF;//qGray(image.pixel(x,y));
                 //graphBWA[x - startOfZoneWidth] += qGray(image.pixel(x,y));
             }
             graphBWA[x - startOfZoneWidth]/=(static_cast<double>(heightOfZone));
            }


            for(int y = startOfZoneHeight; y < image.height()- startOfZoneHeight - 1; y++)
            {
                for(int x = startOfZoneWidth; x < image.width() - startOfZoneWidth-1; x++)
                {
                    graphBWAVertical[y-startOfZoneHeight] += qGray(image.pixel(x,y));//((image.pixel(x,y)))&0x000000FF;
                }
                graphBWAVertical[y-startOfZoneHeight] /=(static_cast<double>(heightOfZone));
            }



            int sizeBWA = graphBWA.size();
            for(int counter = 0; counter < sizeBWA - 1; counter++)
            {
                double delta = graphBWA[counter + 1] - graphBWA[counter];

                graphDerivative[counter] = delta;
            }

            for(int counter = 0; counter < graphBWAVertical.size() - 1;counter++)
            {
                double delta = graphBWAVertical[counter + 1] - graphBWAVertical[counter];

                graphDerivativeVertical[counter] = delta;
            }



            if(!m_previousGraphDerivative.isEmpty() && !m_previousGraphBWA.isEmpty())
            {
                for(int shift = minShiftOfCalc; shift < maxShiftOfCalc; shift++)
                {


                        double int1=0,int2=0/*,int1b = 0, int2b = 0*/;
                        if(shift<0)
                        {
                            for(int i = 0 ; i < graphDerivative.size(); i++)
                            {
                                if((i+shift)>0 && (i+shift)<graphDerivative.size())
                                {
                                    graphDiscrepancy[shift+graphDerivative.size()] += (m_previousGraphDerivative[i] - graphDerivative[i + shift])  *  (m_previousGraphDerivative[i] - graphDerivative[i + shift]);
                                    int1 += m_previousGraphDerivative[i]*m_previousGraphDerivative[i];
                                    int2 += graphDerivative[i + shift]*graphDerivative[i + shift];

                                    graphDiscrepancyColor[shift+graphBWA.size()-1] += (m_previousGraphBWA[i] - graphBWA[i + shift])  *  (m_previousGraphBWA[i] - graphBWA[i + shift]);
//                                    int1b += m_previousGraphBWA[i]*m_previousGraphBWA[i];
//                                    int2b += graphBWA[i + shift]*graphBWA[i + shift];
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

                                    graphDiscrepancyColor[shift + graphBWA.size()-1] += (m_previousGraphBWA[i] - graphBWA[i + shift])*(m_previousGraphBWA[i] - graphBWA[i + shift]);
//                                    int1b += m_previousGraphBWA[i]*m_previousGraphBWA[i];
//                                    int2b += graphBWA[i + shift]*graphBWA[i + shift];
                                }
                            }
                        }
                        //нормируем на интегралы
                        if(int1*int2>0)
                            graphDiscrepancy[shift+graphDerivative.size()] /= (int1*int2);
                        else
                            graphDiscrepancy[shift+graphDerivative.size()] = NAN;//из-за этого в консоли выскакивают nan, но это убирает завалы невязки))

//                        if(int1b*int2b>0)
//                            graphDiscrepancyColor[shift+graphBWA.size()] /= (int1b*int2b);
//                        else
//                            graphDiscrepancyColor[shift+graphBWA.size()] = NAN;//из-за этого в консоли выскакивают nan, но это убирает завалы невязки))



                }
            }


//            int startOfCut = graphDiscrepancy.size()/4;
//            int endOfCut = (graphDiscrepancy.size()*3)/4;
//            if(!graphDiscrepancy.isEmpty())
//            {
//                for(int i = startOfCut ; i < endOfCut; i++)
//                {
//                    cutDiscrepancy[i - startOfCut] = graphDiscrepancy[i];
//                }

//                qreal minValueOfCut = __DBL_MAX__;
//                for(int counter = 0; counter < cutDiscrepancy.size(); counter++)
//                {
//                    if(cutDiscrepancy[counter] < minValueOfCut)
//                    {
//                        minValueOfCut = cutDiscrepancy[counter];
//                        indOfMinCut = counter;
//                    }
//                }

//                int mainX = startOfCut + indOfMinCut;
//                shiftOfDis = mainX - graphDerivative.size();
//            }



            if(!graphDiscrepancy.isEmpty())
            {
                qreal minValueOfDis = __DBL_MAX__;
                qreal minValueOfDisColor = __DBL_MAX__;
                for(int i = minShiftOfCalc + graphDerivative.size()+1; i < maxShiftOfCalc + graphDerivative.size() -1; i++)
                {
                    if(graphDiscrepancy[i] < minValueOfDis)
                    {
                        minValueOfDis = graphDiscrepancy[i];
                        indOfMinDis = i;
                    }

                    if(graphDiscrepancyColor[i] < minValueOfDisColor)
                    {
                        minValueOfDisColor = graphDiscrepancyColor[i];
                        indOfMinDisColor = i;
                    }
                }

                shiftOfDis = indOfMinDis - graphDerivative.size();
                shiftOfDisColor = indOfMinDisColor - graphBWA.size()+1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                if(m_counterFrame > 4)
//                {
//                    if(shiftOfDis == 0 && m_recordStarted)
//                        m_counterZeroes++;

//                    if( (/*m_previousShift > 3 || */m_previousShift < -3) && m_counterZeroes > 25)
//                    {
//                        QImage mergedImage(4000,720,QImage::Format_RGB32);
//                        mergedImage.fill(Qt::black);
//                        QString str = QDir::homePath() + "/mergedImages/" + QString::number(m_counterFrame) + ".bmp";
////                        mergedImage.save(str, "BMP");
//                        QPainter painter(&mergedImage);

//                        if(m_previousShift < -3)
//                        {
//                            int pointerOfDrawing = 0;
//                            for(int i = 0; i < m_counterImageVector; i++)
//                            {
//                                painter.drawImage(pointerOfDrawing, 0, m_mergedImageVector[i]);
//                                pointerOfDrawing += m_mergedImageVector[i].width();
//                            }
//                        }/*else if(m_previousShift > 3)
//                        {
//                            int pointerOfDrawing = mergedImage.width();
//                            for(int i = m_counterImageVector - 1; i > 0 - 1; i--)
//                            {
//                                pointerOfDrawing -= m_mergedImageVector[i].width();
//                                painter.drawImage(pointerOfDrawing, 0, m_mergedImageVector[i]);
//                            }
//                        }*/

//                        mergedImage.save(str, "BMP");

//                        m_mergedImageVector.clear();
//                        m_mergedImageVector.resize(200);

//                        m_previousShift = 0;
//                        m_counterZeroes = 0;
//                        m_recordStarted = false;

//                    }

//                    if(shiftOfDis < -3)
//                    {
//                        QImage cropped;
//                        cropped = image.copy(centerOfImageWidth + shiftOfDis, 0, -shiftOfDis, 720).mirrored(false,true);
//                        m_previousShift = shiftOfDis;
////                        QString str = QDir::homePath() + "/images/" + QString::number(m_counterFrame) + ".bmp";
////                        qDebug() << str;
//                        m_mergedImageVector[m_counterImageVector] = cropped;
//                        m_counterImageVector++;
//                        m_recordStarted = true;
//                    }

////                    if(shiftOfDis > 3)
////                    {
////                        QImage cropped;
////                        cropped = image.copy(centerOfImageWidth, 0, shiftOfDis, 720).mirrored(false,true);
////                        m_previousShift = shiftOfDis;
//////                        QString str = QDir::homePath() + "/images/" + QString::number(m_counterFrame) + ".bmp";
//////                        qDebug() << str;
////                        m_mergedImageVector[m_counterImageVector] = cropped;
////                        m_counterImageVector++;
////                        m_recordStarted = true;
////                    }


//                }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            }


        }
    }while(false);

//    msec = qAbs(QTime::currentTime().msecsTo(t1));
//    t3 = fromMSec(msec);

//    qDebug()<<t3;


    emit frameProcessed(graphDerivative, graphDiscrepancy, m_previousGraphDerivative, shiftOfDis, m_counterFrame, shiftOfDisColor, graphBWA, m_previousGraphBWA , graphDiscrepancyColor);

    m_previousGraphDerivative = graphDerivative;
    m_previousGraphBWA = graphBWA;

    emit shiftReady(shiftOfDis);
}

FrameProcessor::FrameProcessor(QObject *parent)
    : QObject(parent)
{
//   m_previousGraphDerivative.resize(852);
    m_mergedImageVector.resize(200);
}



void MaskProcessor::processMask(QVideoFrame frame, int width, int height)
{
    int widthOfImage = 1280;
    int heightOfImage = 720;

    int widthOfMask;
    int heightOfMask;
    int startOfMaskWidth;
    int startOfMaskHeight;
    if((width == 0) || (height == 0))
    {
        widthOfMask = (widthOfImage*2)/3;
        heightOfMask = (heightOfImage*2)/3;
        startOfMaskWidth = (widthOfImage - widthOfMask)/2;
        startOfMaskHeight = (heightOfImage - heightOfMask)/2;
    }
    else
    {
        widthOfMask = width;
        heightOfMask = height;
        startOfMaskWidth = (widthOfImage - widthOfMask)/2;
        startOfMaskHeight = (heightOfImage - heightOfMask)/2;
    }
    QImage outImage;
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

//            QRgb value = qRgba(255, 0, 255, 0);

//            for(int x = startOfMaskWidth; x < startOfMaskWidth + widthOfMask; x++)
//            {
//                for(int y = startOfMaskHeight; y < startOfMaskHeight + heightOfMask; y++)
//                {
//                    if((x == startOfMaskWidth) || (x == startOfMaskWidth + widthOfMask - 1))
//                        image.setPixel(x,y,value);

//                    if((y == startOfMaskHeight) || (y == startOfMaskHeight + heightOfMask - 1))
//                        image.setPixel(x,y,value);
//                }
//            }
            QPainter painter(&image);
            painter.setBrush(Qt::NoBrush);
            painter.setPen(Qt::red);
            painter.drawRect(startOfMaskWidth,startOfMaskHeight,widthOfMask,heightOfMask);
            painter.end();

            outImage = image;
        }

    }while(false);

    emit maskProcessed(outImage);
}
