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

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
public slots:
    void updatelineedit(int);
    void processChart(QVector<double>);

private:
    Ui::NewPlayer *ui;
    QVideoWidget *m_videoWidget = nullptr;
    QMediaPlayer *m_player = nullptr;
    QVideoProbe *m_videoProbe = nullptr;
    FPSWidget *m_fpsWidget = nullptr;
    QChart *m_graphChart = nullptr;

};
#endif // NEWPLAYER_H
