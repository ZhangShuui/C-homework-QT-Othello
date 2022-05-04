#include "mainboardwindow.h"
#include <QDebug>

MyThread::MyThread ( MainBoardWindow *parent ) : QThread ( parent ) {
    stopped = false;
    this->parent = parent;
}

void MyThread::run() {
    while (parent->gaming) {
        parent->update_valid_pos(DEFAULT);
        parent->check_valid_pos(DEFAULT,parent->nowrole);
        if(parent->valid_pos->empty()){
            parent->nowrole = -parent->nowrole;
            parent->check_valid_pos(DEFAULT,parent->nowrole);
            if(parent->valid_pos->empty())
                parent->gaming=false;
            continue;
        }
        if(parent->nowrole==-parent->h_role){
           emit rop();
           parent->clickable = true;
           parent->nowrole = -parent->nowrole;
        }

    }
    parent->endgame();
    quit();
}

void MyThread::stop() {
    stopped = true;
}
