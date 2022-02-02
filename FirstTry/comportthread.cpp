#include "comportthread.h"
#include <QDebug>
#include <QSerialPort>

ComPortThread::ComPortThread(QObject *parent)
    : QThread{parent}
{
}

ComPortThread::~ComPortThread()
{

}
void ComPortThread::changePort(const QString &portName)
{
    m_portName = portName;

    if (!isRunning())
        start();
    else
        m_cond.wakeOne();



}

void ComPortThread::sendShift(int shift)
{
    m_request = shift;

    if(!isRunning())
        start();
    else
        m_cond.wakeOne();
}



void ComPortThread::run()
{
    bool currentPortNameChanged = false;
    QSerialPort serial;
    QString currentPortName;
    int currentWaitTimeout = 1000;
    int currentRequest;

    while(!m_quit)
    {

        if(currentPortNameChanged)
        {
            serial.close();
            serial.setPortName(currentPortName);

            serial.open(QIODevice::WriteOnly);

        }



            const char requestData = currentRequest;
            qDebug()<< requestData<< " " << currentRequest;
            serial.write(&requestData,1);

            serial.waitForBytesWritten();


        m_mutex.lock();
        m_cond.wait(&m_mutex);
        if (currentPortName != m_portName) {
            currentPortName = m_portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentRequest = m_request;
        m_mutex.unlock();



    }




}




