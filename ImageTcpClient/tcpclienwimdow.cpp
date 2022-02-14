#include "tcpclienwimdow.h"
#include "ui_tcpclienwimdow.h"

TcpClienWimdow::TcpClienWimdow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClienWimdow)
{
    ui->setupUi(this);
}

TcpClienWimdow::~TcpClienWimdow()
{
    delete ui;
}

