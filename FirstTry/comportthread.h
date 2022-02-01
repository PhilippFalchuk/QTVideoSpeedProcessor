#ifndef COMPORTTHREAD_H
#define COMPORTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class ComPortThread : public QThread
{
    Q_OBJECT
public:
    explicit ComPortThread(QObject *parent = nullptr);
    ~ComPortThread();

public slots:
    void changePort(const QString &portName);
    void sendShift(int);

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    void run() override;

    QString m_portName;
    int m_request;
    bool m_quit = false;
    QMutex m_mutex;
    QWaitCondition m_cond;
};

#endif // COMPORTTHREAD_H
