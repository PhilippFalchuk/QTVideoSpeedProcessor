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
    connect(&m_processor, &FrameProcessor::frameProcessed, this, &FPSWidget::frameReady);
    connect(&m_processor, SIGNAL(frameProcessed(QVector<double>)), this, SLOT(frameReady(QVector<double>)));
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

void FPSWidget::frameReady(QVector<double> graphBWA)
{
    emit sendGraph(graphBWA);
}

void FrameProcessor::processFrame(QVideoFrame frame)
{
    int widthOfImage = 1280;
    int widthOfZone = (widthOfImage*14)/24;
    int heightOfImage = 720;
    int heightOfZone = (heightOfImage*14)/24;
    int startOfZoneWidth = (widthOfImage - widthOfZone)/2;
    int startOfZoneHeight = (heightOfImage - heightOfZone)/2;
    QVector<double> graphBWA(widthOfZone);
    do
    {
        if(!frame.map(QAbstractVideoBuffer::ReadOnly))
            break;

        QImage::Format imageFormat =QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
        if(imageFormat != QImage::Format_Invalid)
        {
            QImage image(frame.bits(), frame.width(), frame.height(), imageFormat);
            image = image.convertToFormat(QImage::Format_RGB32);


            for (int y = startOfZoneHeight; y < image.height() - startOfZoneHeight; ++y)
            {
             for(int x = startOfZoneWidth; x < image.width() - startOfZoneWidth - 1; x++)
             {
                 graphBWA[x - startOfZoneWidth] += qGray(image.pixel(x,y));
             }

            }

            for(int counter = 0; counter < graphBWA.size(); counter++)
            {
                graphBWA[counter] = graphBWA[counter]/420;
            }


        }
    }while(false);

    emit frameProcessed(graphBWA);
}
