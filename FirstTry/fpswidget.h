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
    void processFrame(QVideoFrame frame);

signals:
    void frameProcessed(QVector<double>,QVector<double>,QVector<double>, int);

private:
    QVector<double> m_previousGraphDerivative;

};


class FPSWidget : public QLabel
{
    Q_OBJECT

public:
    FPSWidget(QLabel *parent = nullptr);
    ~FPSWidget();

public slots:
    void processFrame(const QVideoFrame &frame);
    void refreshCounter();

signals:
    void frameReady(QVector<double>, QVector<double>, QVector<double>, int);
    void updatelineedit(int);

private:
    qint32 m_fpsCounter;
    QThread m_processorThread;
    FrameProcessor m_processor;
    QColor m_backgroundColor;
    int m_margin;


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};



#endif // FPSWIDGET_H
