#include "mainboardwindow.h"
#include "ui_mainboardwindow.h"
#include <QMessageBox>

#include "QPainter"
#include "QLabel"
#include "QString"
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
    delete label;
    delete ui;
}
void MainBoardWindow::init(int lev){
    this->setFixedSize(this->geometry().size());
    valid_pos = new QVector<QPoint>();
    valid_pos_player = new QVector<QPoint>();
    mythread =new MyThread(this);
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
    painter.setPen(pen);
    if(nowrole == h_role)
    for (int i=0;i<WIDTH;i++) {
        for (int j = 0; j < WIDTH; ++j) {
            QPoint p(i,j);
            for (int k = 0; k < valid_pos_player->length(); ++k) {
                if(p == valid_pos_player->operator[](i))
                    painter.drawEllipse(i*100,j*100,100,100);
            }
        }
    }
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
    check_valid_pos_player(h_role);
    QPoint p = event->pos();
    int m_x = p.x()/100;
    int m_y = p.y()/100;
    QPoint my_step(m_x,m_y);

}

void MainBoardWindow::check_valid_pos(int color){
    while (!valid_pos->empty()) {
        valid_pos->pop_back();
    }
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if(chesses[i][j]!=color)
                continue;
            int k=i;
            int m=j;
            int count=0;
            while (--m>0&&chesses[k][m]==-color) {
                count++;
            }
            if(m<0||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++m<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m>=WIDTH||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>0&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(k>=WIDTH||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>0&&--m>0&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||m<0||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>0&&++m<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||m>=WIDTH||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--m>0&&++k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m<0||k>=WIDTH||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++m<WIDTH&&++k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m>=WIDTH||k>=WIDTH||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos->append(QPoint(k,m));
            }
        }
    }
}

void MainBoardWindow::startgame(){
    mythread->start();


}


void MainBoardWindow::putchess(int color){
    if(level==1){

    }
}


void MainBoardWindow::check_valid_pos_player(int color){
    while (!valid_pos_player->empty()) {
        valid_pos_player->pop_back();
    }
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if(chesses[i][j]!=color)
                continue;
            int k=i;
            int m=j;
            int count=0;
            while (--m>0&&chesses[k][m]==-color) {
                count++;
            }
            if(m<0||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos_player->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++m<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m>=WIDTH||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos_player->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>0&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos_player->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(k>=WIDTH||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos_player->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>0&&--m>0&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||m<0||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos_player->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>0&&++m<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||m>=WIDTH||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos_player->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--m>0&&++k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m<0||k>=WIDTH||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos_player->append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++m<WIDTH&&++k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m>=WIDTH||k>=WIDTH||chesses[k][m]==color||!count)
                continue;
            else {
                valid_pos_player->append(QPoint(k,m));
            }
        }
    }
}


void MainBoardWindow::endgame(){

};










