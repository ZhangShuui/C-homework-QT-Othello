#include "mainboardwindow.h"
#include "ui_mainboardwindow.h"
#include <QMessageBox>
#include <QtDebug>
#include "QPainter"
#include "QLabel"
#include "QString"
#include <stdlib.h>
#include <time.h>
#define PLAYER 100
#define AI 101
MainBoardWindow::MainBoardWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainBoardWindow)
{
    ui->setupUi(this);
}
MainBoardWindow::MainBoardWindow(QWidget *parent,int level) :
    QMainWindow(parent),
    ui(new Ui::MainBoardWindow)
{
    ui->setupUi(this);
    Level = level;
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            chesses[i][j] = NO_CHESS;
        }
    }
    for (int i = 3; i < 5; ++i) {
        for (int j = 3; j < 5; ++j) {
            chesses[i][j] = i==j?BLACK:WHITE;
        }
    }
    this->level =level;
    nowrole =BLACK;
    init(level);
}

MainBoardWindow::~MainBoardWindow()
{
    delete valid_pos;
    delete valid_pos_robot;
    delete valid_pos_player;
    delete label;
    delete role;
    delete ui;
}
void MainBoardWindow::init(int lev){
    this->setFixedSize(this->geometry().size());
    h_role =NO_CHESS;
    role = new QLabel();
    valid_pos = new QVector<QPoint>();
    valid_pos_player = new QVector<QPoint>();
    valid_pos_robot = new QVector<QPoint>();
    mythread =new MyThread(this);
    connect(mythread,SIGNAL(rop()),this,SLOT(putchess()));
    gaming = false;
    clickable = false;
    label=new QLabel(this);
    label->setVisible(true);
    switch (lev) {
    case 1:
        label->setText("EASY MODE");
        break;
    case 2:
        label->setText("MEDIUM MODE");
        break;
    case 3:
        label->setText("HARD MODE");
        break;
    }
    QFont ft("Microsoft YaHei",18);
    label->setStyleSheet("color:black;");
    label->setFont(ft);
    label->setGeometry(830,50,220,80);
}
void MainBoardWindow::paintEvent(QPaintEvent *event){
    QPixmap My_Back(800,800);
    My_Back.fill(Qt::white);
    QPainter painter(&My_Back);
    QPixmap pix;
    pix.load(":/pic/chessboard.jpeg");
    painter.drawPixmap(0,0,800,800,pix);
    for (int i = 0; i <= 8; ++i) {
        QPen pen;
        pen.setColor(QColor(0,0,0));
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawLine(i*100,0,i*100,800);
        painter.drawLine(0,i*100,800,i*100);
    }
    QPixmap pixx[3];
    for(int i=0;i<3;++i){
        switch (i) {
        case 0:
            pixx[i].load(":/pic/blackchess.png");
            break;
        case 1:
            pixx[i].load(":/pic/whitechess.png");
            break;
        default:
            pixx[i].load(":/pic/nochess.png");
            break;
        }
    }
    for (int i=0;i<WIDTH;i++) {
        for (int j = 0; j < WIDTH; ++j) {
            QPixmap pix;
            switch (chesses[i][j]) {
            case BLACK:
                painter.drawPixmap(i*100,j*100,100,100,pixx[0]);
                break;
            case WHITE:
                painter.drawPixmap(i*100,j*100,100,100,pixx[1]);
                break;
            default:
                painter.drawPixmap(i*100,j*100,100,100,pixx[2]);
                break;
            }
        }
    }
    QPen pen;
    QVector<qreal>dashes;
    qreal space = 4;
    dashes << 3 << space;
    pen.setDashPattern(dashes);
    pen.setWidth(4);
    painter.setPen(pen);
    if(nowrole == h_role){
        check_valid_pos(PLAYER,h_role);
        for (int i=0;i<WIDTH;i++) {
            for (int j = 0; j < WIDTH; ++j) {
                QPoint p(i,j);
//                for (int k = 0; k < valid_pos_player->length(); ++k) {
//                    if(p == valid_pos_player->operator[](i)){
//                           painter.drawEllipse(i*100,j*100,100,100);
//                           qDebug() << p;
//                    }
//                }
                for (auto k = valid_pos_player->begin();k!=valid_pos_player->end();k++) {
                    if(p == *k){
                        painter.drawEllipse(i*100,j*100,100,100);
                        qDebug() << p;
                    }
                }
            }
        }
    }
    qDebug() << "finish one draw";
    painter.end();
    painter.begin(this);
    painter.drawPixmap(0,0,My_Back);
    painter.end();
}

void MainBoardWindow::on_pushButton_clicked()
{
    if(gaming)
        return;
    int is_black = QMessageBox::question(this,tr(""),tr("作为黑棋先手？"),QMessageBox::Yes,QMessageBox::No);
    h_role = is_black==QMessageBox::Yes? BLACK:WHITE;
    clickable = h_role==BLACK? true:false;
    gaming = true;
    startgame();
}
//下面是投降部分代码
void MainBoardWindow::on_pushButton_3_clicked()
{
    if(!gaming){
        int ret2 = QMessageBox::information(this, tr("注意"),
                                                tr("游戏未开始！无法进行此操作"), QMessageBox::Ok);
    }
    int ret23 = QMessageBox::information(this, tr("注意"),
                                            tr("游戏结束！电脑胜利"), QMessageBox::Ok);
    gaming = false;
    clickable = false;
}
//下面是悔棋部分代码,已放弃
void MainBoardWindow::on_pushButton_2_clicked()
{
//    if(!gaming){
//        int ret2 = QMessageBox::information(this, tr("注意"),
//                                                tr("游戏未开始！无法进行此操作"), QMessageBox::Ok);
//    }
}

void MainBoardWindow::on_pushButton_4_clicked()
{
    gaming =false;
    this->destroy();
}

void MainBoardWindow::mousePressEvent(QMouseEvent *event){
    if(!clickable)
        return;
    check_valid_pos(PLAYER,h_role);
    QPoint p = event->pos();
    int m_x = p.x()/100;
    int m_y = p.y()/100;
    qDebug()<<"can"<<endl;
    QPoint my_step(m_x,m_y);
    for (auto i = valid_pos_player->begin(); i != valid_pos_player->end(); ++i) {
        if(*i == my_step){
            qDebug()<<m_x<<","<<m_y<<endl;
            chesses[(*i).x()][(*i).y()]= h_role;
            reverse(my_step.x(),my_step.y());
            update_valid_pos(PLAYER);
            repaint();
            nowrole = -nowrole;
            clickable=false;
            break;
        }
    }
}

void MainBoardWindow::check_valid_pos(int who, int color){
    QVector<QPoint>* valid_pos;
    switch (who) {
    case DEFAULT: valid_pos=this->valid_pos;
        break;
    case PLAYER: valid_pos = this->valid_pos_player;
        break;
    case AI: valid_pos = this->valid_pos_robot;
    }
    while (!valid_pos->empty()) {
        valid_pos->pop_back();
    }
    QVector<QPoint> valid;
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            bool yes=false;
            if(chesses[i][j]!=color)
                continue;
            int k=i;
            int m=j;
            int count=0;
            while (--m>0&&chesses[k][m]==-color) {
                count++;
            }
            if(m<0||chesses[k][m]==color||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++m<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m>=WIDTH||chesses[k][m]==color||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>0&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||chesses[k][m]==color||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(k>=WIDTH||chesses[k][m]==color||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>0&&--m>0&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||m<0||chesses[k][m]==color||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>0&&++m<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||m>=WIDTH||chesses[k][m]==color||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--m>0&&++k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m<0||k>=WIDTH||chesses[k][m]==color||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++m<WIDTH&&++k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m>=WIDTH||k>=WIDTH||chesses[k][m]==color||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            for (auto it = valid.begin();it != valid.end();it++) {
                bool exist = false;
                for (auto ij = valid_pos->begin();ij != valid_pos->end();ij++) {
                    if(*ij == *it){
                        exist =true;
                        break;
                    }
                }
                if(!exist)
                    valid_pos->append(*it);
            }
        }
    }
}

void MainBoardWindow::startgame(){
    mythread->start();
}


void MainBoardWindow::putchess(){
    srand(unsigned(time(NULL)));
    if(level==1){
        check_valid_pos(AI,-h_role);
        if(valid_pos_robot->length()){
            int k = rand()%valid_pos_robot->length();
            QPoint p;
            for (auto it = valid_pos_robot->begin();it!= valid_pos_robot->end();it++) {
                if(it == valid_pos_robot->begin()+k){
                    p = *it;
                    break;
                }
            }
            chesses[p.x()][p.y()] = -h_role;
            reverse(p.x(),p.y());
            update_valid_pos(AI);
            qDebug()<<p.x()<<","<<p.y()<<endl;
        }
        repaint();
    }
}





void MainBoardWindow::endgame(){

};


void MainBoardWindow::reverse(int i, int j){
    int k = i;
    int m = j;
    int color = chesses[i][j];
    int count = 0;
//    QVector<QPoint>* can_point = new QVector<QPoint>();
    while (--m>0&&chesses[k][m]==-color) {
        count++;
    }
    if(m<0||chesses[k][m]==NO_CHESS||!count)
        ;
    else {
        while (++m != j) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (++m<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(m>=WIDTH||chesses[k][m]==NO_CHESS||!count)
        ;
    else {
        while (--m != j) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--k>0&&chesses[k][m]==-color) {
        count++;
    }
    if(k<0||chesses[k][m]==NO_CHESS||!count)
        ;
    else {
        while (++k != i) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (++k<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(k>=WIDTH||chesses[k][m]==NO_CHESS||!count)
        ;
    else {
        while (--k != i) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--k>0&&--m>0&&chesses[k][m]==-color) {
        count++;
    }
    if(k<0||m<0||chesses[k][m]==NO_CHESS||!count)
        ;
    else {
        while (++k != i&&++m != j) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--k>0&&++m<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(k<0||m>=WIDTH||chesses[k][m]==NO_CHESS||!count)
        ;
    else {
        while (++k != i&& --m != j) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--m>0&&++k<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(m<0||k>=WIDTH||chesses[k][m]==NO_CHESS||!count)
        ;
    else {
        while (++m != j&& --k != i) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (++m<WIDTH&&++k<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(m>=WIDTH||k>=WIDTH||chesses[k][m]==color||!count)
        ;
    else {
        while (--k != i&& --m != j) {
            chesses[k][m] = color;
        }
    }

}


void MainBoardWindow::update_valid_pos(int who){
    QVector<QPoint>* valid_pos;
    switch (who) {
    case PLAYER: valid_pos = valid_pos_player;
    case AI: valid_pos = valid_pos_robot;
    case DEFAULT: valid_pos = this->valid_pos;
    }
    while (!valid_pos->empty()) {
        valid_pos->pop_back();
    }
}




