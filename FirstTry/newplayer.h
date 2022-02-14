#ifndef NEWPLAYER_H
#define NEWPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include "threadhandler.h"
#include "comportthread.h"
#include "imagetcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class NewPlayer; }
QT_END_NAMESPACE

class NewPlayer : public QWidget
{
    Q_OBJECT

public:
    NewPlayer(QWidget *parent = nullptr);
    ~NewPlayer();

signals:
    void zoneChanged(int,int,int, bool);
    void portChanged(const QString &portName);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void on_editZone_clicked();

    void on_portPushButton_clicked();

public slots:
    void updatelineedit(int);
    void processChart(QVector<double> graphDerivative, QVector<double> graphDiscrepancy,QVector<double> graphPreviousDerivative, int shift, int framesCount);
    void displayMask(QImage);
    void onlyTextShift(int);

private:
    Ui::NewPlayer *ui;
    QVideoWidget *m_videoWidget = nullptr;
    QMediaPlayer *m_player = nullptr;
    QVideoProbe *m_videoProbe = nullptr;
    ThreadHandler *m_threadHandler = nullptr;
    QChart *m_graphChartDerivative = nullptr;
    QChart *m_graphChartDiscrepancy = nullptr;
    QChart *m_graphChartPreviousDerivative = nullptr;
    QChart *m_graphChartColor = nullptr;
    QChart *m_graphChartDiscrepancyColor = nullptr;
    QChart *m_graphChartPreviousColor = nullptr;
    QChart *m_bufferChart = nullptr;
    QLineSeries *m_series = nullptr;
    QLineSeries *m_bufferSeries = nullptr;
    QLineSeries *m_bufferSeriesColor = nullptr;
    QLineSeries *m_seriesDerivative = nullptr;
    QLineSeries *m_seriesDiscrepancy = nullptr;
    QLineSeries *m_seriesPreviousDerivative = nullptr;
    QLineSeries *m_seriesColor = nullptr;
    QLineSeries *m_seriesDiscrepancyColor = nullptr;
    QLineSeries *m_seriesPreviousColor = nullptr;
    int m_bufferCounter = 0;
    QChartView *m_bufferChartView = nullptr;
    QValueAxis *m_axisY = nullptr;
    QValueAxis *m_axisX = nullptr;
    QValueAxis *m_axisDerX = nullptr;
    QValueAxis *m_axisPrevDerX = nullptr;
    QValueAxis *m_axisDisX = nullptr; 
    QValueAxis *m_axisColorX = nullptr;
    QValueAxis *m_axisPrevColorX = nullptr;
    QValueAxis *m_axisDisColorX = nullptr;

    ComPortThread m_comThread;
    ImageTcpServer m_imageTcpServer;


    QString m_pathToUrl = QDir::currentPath() + "/urlEdit.txt";
    QString m_url;




    void shiftBuffer(int, int);

};
#endif // NEWPLAYER_H
