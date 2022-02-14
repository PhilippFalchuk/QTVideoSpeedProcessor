#ifndef TCPCLIENWIMDOW_H
#define TCPCLIENWIMDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TcpClienWimdow; }
QT_END_NAMESPACE

class TcpClienWimdow : public QMainWindow
{
    Q_OBJECT

public:
    TcpClienWimdow(QWidget *parent = nullptr);
    ~TcpClienWimdow();

private:
    Ui::TcpClienWimdow *ui;
};
#endif // TCPCLIENWIMDOW_H
