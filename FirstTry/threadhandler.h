#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include <QThread>
#include <QVideoFrame>
#include <QWidget>
#include <QMargins>
#include <QLabel>
#include <QtCharts>

class FrameProcessor : public QObject
{
    Q_OBJECT

public:
    FrameProcessor(QObject *parent = nullptr);
public slots:
    void processFrame(QVideoFrame frame, int, int, int);

signals:
    void frameProcessed(QVector<double>,QVector<double>,QVector<double>, int, int, int,QVector<double>,QVector<double>,QVector<double>);

private:
    QVector<double> m_previousGraphDerivative;
    QVector<double> m_previousGraphBWA;
    int m_counterFrame = 0;
    int m_previousShift = 0;
    int m_counterImageVector = 0;
    int m_counterZeroes = 0;
    bool m_recordStarted = false;
    QVector<QImage> m_mergedImageVector;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MaskProcessor : public QObject
{
    Q_OBJECT

public slots:
    void processMask(QVideoFrame, int, int);

signals:
    void maskProcessed(QImage);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ThreadHandler : public QLabel
{
    Q_OBJECT

public:
    ThreadHandler(QLabel *parent = nullptr);
    ~ThreadHandler();

public slots:
    void processFrame(const QVideoFrame &frame);
    void refreshCounter();
    void setZone(int, int, int);

signals:
    void frameReady(QVector<double>, QVector<double>, QVector<double>, int, int, int,QVector<double>,QVector<double>,QVector<double>);
    void maskReady(QImage);
    void updatelineedit(int);

private:
    qint32 m_fpsCounter;
    QThread m_processorThread;
    QThread m_maskThread;
    FrameProcessor m_processor;
    MaskProcessor m_maskProcessor;

    QColor m_backgroundColor;
    int m_margin;
    int m_zoneWidth = 0;
    int m_zoneHeight = 0;
    int m_widthOfDis = 0;


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};



#endif // THREADHANDLER_H
