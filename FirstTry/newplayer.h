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
    void processChart(QVector<double>, QVector<double>, QVector<double>, int);

private:
    Ui::NewPlayer *ui;
    QVideoWidget *m_videoWidget = nullptr;
    QMediaPlayer *m_player = nullptr;
    QVideoProbe *m_videoProbe = nullptr;
    FPSWidget *m_fpsWidget = nullptr;
    QChart *m_graphChartDerivative = nullptr;
    QChart *m_graphChartDiscrepancy = nullptr;
    QChart *m_graphChartPreviousDerivative = nullptr;
    QChart *m_bufferChart = nullptr;
    QLineSeries *m_series = nullptr;
    QLineSeries *m_bufferSeries = nullptr;
    int m_bufferCounter = 0;
    QChartView *m_bufferChartView = nullptr;

    void shiftBuffer(int);

};
#endif // NEWPLAYER_H
