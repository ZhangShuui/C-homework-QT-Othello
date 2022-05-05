#include "mainwindow.h"
#include <QApplication>
#include <mcts_pyqt.h>
#include <QVariant>
#include <QDebug>
#include <time.h>
int asss[8][8];
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

//    for (int i=3;i<5;i++) {
//        for (int j =3 ;j<5;j++) {
//            asss[i][j] = i==j?1:-1;
//        }
//    }
//    QVariantList* ss =new QVariantList();
//    QString* str=new QString();
//    for (int i=0;i<8;i++) {
//        for (int j=0;j<8;j++) {
//            str->append(QString("%1 ").arg(asss[i][j]));
//        }
//    }
//    str->append('1');
//    qDebug()<<*str;
//    QVariant sx =QVariant(*str);
//    ss->append(sx);
//    QVariant sb = QVariant("");
//    QVariant* back =new QVariant(sb);
//    MCTS_pyqt pp("main");
//    qDebug()<<pp.callPyFunc("use",*ss,back);
//    qDebug()<<*back;
//    return 0;
}
