#include "mainboardwindow.h"
#include <QDebug>

MyThread::MyThread ( MainBoardWindow *parent ) : QThread ( parent ) {
    stopped = false;
    this->parent = parent;
}

void MyThread::run() {
    int nowrole = parent->nowrole;
    int h_role = parent->h_role;
    bool clickable = parent->clickable;
    bool gaming = parent->gaming;
    while (parent->gaming) {
        parent->check_valid_pos(nowrole);
        if(parent->valid_pos->empty()){
            nowrole = -nowrole;
            parent->check_valid_pos(nowrole);
            if(parent->valid_pos->empty())
                gaming=false;
            continue;
        }
        if(nowrole!=h_role){
           parent->putchess(nowrole);
           clickable =true;
        }

    }
    parent->endgame();
    stop();
    quit();
}

void MyThread::stop() {
    stopped = true;
}
