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


private:
    Ui::NewPlayer *ui;
    QVideoWidget *m_videoWidget = nullptr;
    QMediaPlayer *m_player = nullptr;
    QVideoProbe *m_videoProbe = nullptr;
    ThreadHandler *m_threadHandler = nullptr;

    quint32 m_bufferCounter = 0;


    ComPortThread m_comThread;
    ImageTcpServer m_imageTcpServer;


    QString m_pathToUrl = QDir::currentPath() + "/urlEdit.txt";
    QString m_url;




    void shiftBuffer(int, int);

};
#endif // NEWPLAYER_H
