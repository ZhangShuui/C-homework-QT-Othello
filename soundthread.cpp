#include "mainboardwindow.h"
#include <QDebug>

SoundThread::SoundThread ( MainBoardWindow *parent ) : QThread ( parent ) {
    stopped = false;
    this->parent = parent;

}

void SoundThread::run() {

}

void SoundThread::stop() {
    stopped = true;
}
