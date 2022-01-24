#ifndef FPSWIDGET_H
#define FPSWIDGET_H

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
    void processFrame(QVideoFrame frame, int, int);

signals:
    void frameProcessed(QVector<double>,QVector<double>,QVector<double>, int, int);

private:
    QVector<double> m_previousGraphDerivative;
    int counterFrame = 0;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class FrameProcessorColor : public QObject
{
    Q_OBJECT

public:
    FrameProcessorColor(QObject *parent = nullptr);
public slots:
    void processFrameColor(QVideoFrame frame, int, int);

signals:
    void frameProcessedColor(QVector<double>,QVector<double>,QVector<double>, int);

private:
    QVector<double> m_previousGraphDerivative;
    int counterframe = 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class FPSWidget : public QLabel
{
    Q_OBJECT

public:
    FPSWidget(QLabel *parent = nullptr);
    ~FPSWidget();

public slots:
    void processFrame(const QVideoFrame &frame);
    void refreshCounter();
    void setZone(int, int);

signals:
    void frameReady(QVector<double>, QVector<double>, QVector<double>, int, int);
    void frameReadyColor(QVector<double>, QVector<double>, QVector<double>, int);
    void updatelineedit(int);

private:
    qint32 m_fpsCounter;
    QThread m_processorThread;
    QThread m_processorThreadColor;
    FrameProcessor m_processor;
    FrameProcessorColor m_processorColor;
    QColor m_backgroundColor;
    int m_margin;
    int m_zoneWidth = 0;
    int m_zoneHeight = 0;


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};



#endif // FPSWIDGET_H
