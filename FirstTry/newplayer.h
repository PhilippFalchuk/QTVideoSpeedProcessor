#ifndef NEWPLAYER_H
#define NEWPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include "fpswidget.h"

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
    void zoneChanged(int,int);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void on_editZone_clicked();

public slots:
    void updatelineedit(int);
    void processChart(QVector<double>, QVector<double>, QVector<double>, int, int);
    void processChartColor(QVector<double>, QVector<double>, QVector<double>, int);
    void displayMask(QImage);

private:
    Ui::NewPlayer *ui;
    QVideoWidget *m_videoWidget = nullptr;
    QMediaPlayer *m_player = nullptr;
    QVideoProbe *m_videoProbe = nullptr;
    FPSWidget *m_fpsWidget = nullptr;
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
    int m_bufferCounter = 0;
    int m_bufferCounterColor = 0;
    QChartView *m_bufferChartView = nullptr;
    QValueAxis *m_axisY = nullptr;
    QValueAxis *m_axisX = nullptr;
    QValueAxis *m_axisDerX = nullptr;
    QValueAxis *m_axisPrevDerX = nullptr;
    QValueAxis *m_axisDisX = nullptr;


    void shiftBuffer(int, int);
    void shiftBufferColor(int);

};
#endif // NEWPLAYER_H
