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
    void processFrame(QVideoFrame frame, int, int, int,bool);

signals:
    void frameProcessed(QVector<double>,QVector<double>,QVector<double>, int, int);
    void shiftReady(int);

private:
    QVector<double> m_previousGraphDerivative;
    //QVector<double> m_previousGraphBWA;
    int m_counterFrame = 0;
    int m_previousShift = 0;
    int m_counterImageVector = 0;
    int m_counterZeroes = 0;
    bool m_minusRecordStarted = false;
    bool m_plusRecordStarted = false;
    QVector<QImage> m_mergedImageVector;
    QString imagePathStr = QDir::currentPath() + "/";
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

    FrameProcessor m_processor;

public slots:
    void processFrame(const QVideoFrame &frame);
    void refreshCounter();
    void setZone(int, int, int, bool);

signals:
    void frameReady(QVector<double>,QVector<double>,QVector<double>, int, int);
    void maskReady(QImage);
    void updatelineedit(int);

private:
    qint32 m_fpsCounter;
    QThread m_processorThread;
    QThread m_maskThread;
    MaskProcessor m_maskProcessor;

    QColor m_backgroundColor;
    int m_margin;
    int m_zoneWidth = 0;
    int m_zoneHeight = 0;
    int m_widthOfDis = 0;
    bool m_saverFlag = false;


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};



#endif // THREADHANDLER_H
