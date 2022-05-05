#include "mainboardwindow.h"
#include <QDebug>
#include "time.h"
TimeThread::TimeThread ( MainBoardWindow *parent ) : QThread ( parent ) {
    stopped = false;
    this->parent = parent;

}

void TimeThread::run() {
    while (parent->gaming) {
        int now = parent->nowrole;
        int count =0;
        emit record();
        int now_time = time(0);
        int end_time = time(0);
        while (parent->nowrole == now) {
            end_time = time(0);
            if(end_time-now_time-1>count){
                count++;
                parent->time_count +=1;
                emit record();
            }
            if(parent->nowrole != now)
                break;
        }
        parent->time_count =0;
    }

//    parent->endgame();
    quit();
}

void TimeThread::stop() {
    stopped = true;
}
