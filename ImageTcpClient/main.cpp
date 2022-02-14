#include "tcpclienwimdow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpClienWimdow w;
    w.show();
    return a.exec();
}
