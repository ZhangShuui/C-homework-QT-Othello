#include "mainboardwindow.h"
#include <QDebug>
#include "time.h"
AiThread::AiThread ( MainBoardWindow *parent ) : QThread ( parent ) {
    stopped = false;
    this->parent = parent;

}

void AiThread::run() {
    QString* str=new QString();
    for (int i=0;i<WIDTH;i++) {
        for (int j=0;j<WIDTH;j++) {
            str->append(QString("%1 ").arg(parent->chesses[i][j]));
        }
    }
    int col = -parent->h_role==BLACK?1:-1;
    str->append(QString("%1").arg(col));
    QVariantList* ql =new QVariantList();
    ql->append(*str);
    QVariant* bak = new QVariant(" ");
    bool okk = parent->Hard_AI->callPyFunc("use",*ql, bak);
    if(bak==nullptr){
        qDebug()<<*str;
        qDebug()<<"wandanle";
    }else if (!okk) {
        qDebug()<<*str;
        qDebug()<<"wandanle";
    }
    else {
        qDebug()<<*bak;
        char x = bak->toString().toStdString().at(0);
        char y = bak->toString().toStdString().at(1);
        int xx = x-'A';
        int yy = y-'1';
        emit reeee(10*yy+xx);
    }

    quit();
}

void AiThread::stop() {
    stopped = true;
}
