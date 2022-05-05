#include "mainboardwindow.h"
#include "ui_mainboardwindow.h"
#include <QMessageBox>
#include <QtDebug>
#include "QPainter"
#include "QLabel"
#include "QString"
#include <stdlib.h>
#include <time.h>
#include "mcts_pyqt.h"
#include <QToolButton>
#include <QSound>
#include <QSoundEffect>
#define PLAYER 100
#define AI 101
#define PAINT 102
MainBoardWindow::MainBoardWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainBoardWindow)
{
    ui->setupUi(this);
}
//player 鼠标点击事件使用palyer
//defualt 单独线程使用默认
//AI 电脑使用robot
//paint paintevent使用paint
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
    delete valid_pos_paint;
    delete label;
    delete now_role;
    delete ui;
}
void MainBoardWindow::init(int lev){
    this->setFixedSize(this->geometry().size());
    putting_chess= false;
    nowrole = BLACK;
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
    playing = false;
    h_role =NO_CHESS;
    time_count = 0;
    valid_pos = new QVector<QPoint>();
    valid_pos_player = new QVector<QPoint>();
    valid_pos_robot = new QVector<QPoint>();
    valid_pos_paint = new QVector<QPoint>();
    timethread = new TimeThread(this);
    mythread =new MyThread(this);
    aithread = new AiThread(this);

    effect=new QSoundEffect();
    effect->setSource(QUrl::fromLocalFile(":/pic/bgm2.wav"));
    effect->setLoopCount(100);  //循环次数
    effect->setVolume(0.7f);

    connect(mythread,SIGNAL(rop()),this,SLOT(putchess()));
    connect(mythread,SIGNAL(repp()),this,SLOT(endgame()));
    connect(timethread,SIGNAL(record()),this,SLOT(update_time()));
    connect(aithread,SIGNAL(reeee(int)),this,SLOT(update_board(int)));
    connect(ui->soundButton,&QToolButton::clicked,this,[=](){
        playing = !playing;
        if(playing){
            ui->soundButton->setIcon(QIcon(":/pic/soundon.png"));
            effect->play();
        }else {
            ui->soundButton->setIcon(QIcon(":/pic/soundoff.png"));
            effect->stop();
        }
    });
    gaming = false;
    clickable = false;
    label=new QLabel(this);
//    time_label =new QLabel(this);
//    time_label->setVisible(true);
    label->setVisible(true);

    now_role = new QLabel(this);
    now_role->setVisible(true);
    QFont ftt("Microsoft YaHei",16);
    now_role->setStyleSheet("color:black;");
    now_role->setFont(ftt);



    QFont fttt("Microsoft YaHei",16);
//    time_label->setStyleSheet("color:black;");
//    time_label->setFont(fttt);
    now_role->setGeometry(820,150,200,80);
    now_role->setText(" ");

    switch (lev) {
    case 1:
        label->setText("EASY MODE");
        break;
    case 2:
        label->setText("MED MODE");
        break;
    case 3:
        label->setText("HARD MODE");
        Hard_AI = new MCTS_pyqt("main");
        break;
    }
    QFont ft("Microsoft YaHei",20);
    label->setStyleSheet("color:black;");
    label->setFont(ft);
    label->setGeometry(820,20,220,80);
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
            pixx[i].load(":/pic/black.png");
            break;
        case 1:
            pixx[i].load(":/pic/circle.png");
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

    if(nowrole == h_role){
        QPen pen;
        QVector<qreal>dashes;
        qreal space = 4;
        dashes << 3 << space;
        pen.setDashPattern(dashes);
        pen.setWidth(4);
        painter.setPen(pen);
        check_valid_pos(PAINT,h_role);

        for (auto k = valid_pos_paint->begin();k!=valid_pos_paint->end();k++) {

            int x = (*k).x();
            int y = (*k).y();
            painter.drawEllipse(x*100,y*100,100,100);
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
    effect->stop();
    this->destroy();
}

void MainBoardWindow::mousePressEvent(QMouseEvent *event){
    if(!clickable)
        return;
     (PLAYER);
    check_valid_pos(PLAYER,h_role);
    QPoint p = event->pos();
    int m_x = p.x()/100;
    int m_y = p.y()/100;

    QPoint my_step(m_x,m_y);
    for (auto i = valid_pos_player->begin(); i != valid_pos_player->end(); ++i) {
        if(*i == my_step){
            chesses[(*i).x()][(*i).y()]= h_role;
            reverse(my_step.x(),my_step.y());
             (PLAYER);
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
        break;
    case PAINT: valid_pos = this->valid_pos_paint;
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
            while (--m>=0&&m>=0&&chesses[k][m]==-color) {
                count++;
            }
            if(m<0||chesses[k][m]!=NO_CHESS||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++m<WIDTH&&m!=WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m>=WIDTH||chesses[k][m]!=NO_CHESS||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>=0&&k>=0&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||chesses[k][m]!=NO_CHESS||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++k<WIDTH&&k!=WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(k>=WIDTH||chesses[k][m]!=NO_CHESS||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>=0&&--m>=0&&k>=0&&m>=0&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||m<0||chesses[k][m]!=NO_CHESS||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--k>=0&&++m<WIDTH&&k>=0&&m<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(k<0||m>=WIDTH||chesses[k][m]!=NO_CHESS||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (--m>=0&&++k<WIDTH&&m>=0&&k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m<0||k>=WIDTH||chesses[k][m]!=NO_CHESS||!count)
                ;
            else {
                valid.append(QPoint(k,m));
            }
            k=i;
            m=j;
            count=0;
            while (++m<WIDTH&&++k<WIDTH&&m<WIDTH&&k<WIDTH&&chesses[k][m]==-color) {
                count++;
            }
            if(m>=WIDTH||k>=WIDTH||chesses[k][m]!=NO_CHESS||!count)
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
    timethread->start();
}


void MainBoardWindow::putchess(){
    putting_chess =true;
    repaint();
    srand(unsigned(time(NULL)));
    if(level==1){
         (AI);
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
             (AI);

        }
        putting_chess = false;
        clickable = true;
        nowrole = -nowrole;
        repaint();
    }else if(level == 3){
        aithread->start();
//        QString* str=new QString();
//        for (int i=0;i<WIDTH;i++) {
//            for (int j=0;j<WIDTH;j++) {
//                str->append(QString("%1 ").arg(chesses[i][j]));
//            }
//        }
//        int col = -h_role==BLACK?1:-1;
//        str->append(QString("%1").arg(col));
//        QVariantList* ql =new QVariantList();
//        ql->append(*str);
//        QVariant* bak = new QVariant(" ");
//        bool okk = Hard_AI->callPyFunc("use",*ql, bak);
//        if(bak==nullptr){
//            qDebug()<<*str;
//            qDebug()<<"wandanle";
//        }else if (!okk) {
//            qDebug()<<*str;
//            qDebug()<<"wandanle";
//        }
//        else {
//            qDebug()<<*bak;
//            char x = bak->toString().toStdString().at(0);
//            char y = bak->toString().toStdString().at(1);
//            int xx = x-'A';
//            int yy = y-'1';
//            chesses[yy][xx] = -h_role;
//            reverse(yy,xx);
//            repaint();
//        }
    }else if(level ==2){
        int maxscore=0;
        check_valid_pos(AI,-h_role);
        QPoint p;
        for (auto it = valid_pos_robot->begin();it != valid_pos_robot->end();it++) {
            int score = evaluate(*it);
            if(score >= maxscore){
                p=*it;
            }
        }
        chesses[p.x()][p.y()] = -h_role;
        reverse(p.x(),p.y());
        repaint();
        putting_chess = false;
        clickable = true;
        nowrole = -nowrole;
    }

    repaint();
}





void MainBoardWindow::endgame(){
    int white = 0;
    int black = 0;
    for (int i =0 ;i< WIDTH;++i) {
        for (int j = 0;j<WIDTH;++j) {
            if(chesses[i][j]==BLACK)
                black++;
            else if(chesses[i][j]==WHITE){
                white++;
            }
        }
    }
    if(black>white){
        int ret = QMessageBox::information(this,tr("游戏结束！"),tr("游戏结束，黑棋胜利"),QMessageBox::Ok);
    }else if(black<white){
        int ret = QMessageBox::information(this,tr("游戏结束！"),tr("游戏结束，白棋胜利"),QMessageBox::Ok);
    }else {
        int ret = QMessageBox::information(this,tr("游戏结束！"),tr("游戏结束，平局"),QMessageBox::Ok);
    }
    delete valid_pos;
    delete valid_pos_robot;
    delete valid_pos_player;
    delete valid_pos_paint;
    delete effect;
    delete now_role;
    init();
};


void MainBoardWindow::reverse(int i, int j){
    int k = i;
    int m = j;
    int color = chesses[i][j];
    int count = 0;
//    QVector<QPoint>* can_point = new QVector<QPoint>();
    while (--m>=0&&chesses[k][m]==-color) {
        count++;
    }
    if(m<0||chesses[k][m]!=color||!count)
        ;
    else {
        while (++m != j) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (++m<WIDTH&&m<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(m>=WIDTH||chesses[k][m]!=color||!count)
        ;
    else {
        while (--m != j) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--k>=0&&k>=0&&chesses[k][m]==-color) {
        count++;
    }
    if(k<0||chesses[k][m]!=color||!count)
        ;
    else {
        while (++k != i) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (++k<WIDTH&&k<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(k>=WIDTH||chesses[k][m]!=color||!count)
        ;
    else {
        while (--k != i) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--k>=0&&--m>=0&&k>=0&&m>=0&&chesses[k][m]==-color) {
        count++;
    }
    if(k<0||m<0||chesses[k][m]!=color||!count)
        ;
    else {
        while (++k != i&&++m != j) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--k>=0&&++m<WIDTH&&k>=0&&m<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(k<0||m>=WIDTH||chesses[k][m]!=color||!count)
        ;
    else {
        while (++k != i&& --m != j) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--m>=0&&++k<WIDTH&&m>=0&&k<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(m<0||k>=WIDTH||chesses[k][m]!=color||!count)
        ;
    else {
        while (++m != j&& --k != i) {
            chesses[k][m] = color;
        }
    }
    k=i;
    m=j;
    count=0;
    while (++m<WIDTH&&++k<WIDTH&&m<WIDTH&&k<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(m>=WIDTH||k>=WIDTH||chesses[k][m]!=color||!count)
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


int MainBoardWindow::evaluate(QPoint p){
    int score =0;
    if(p.x()==0||p.x()==WIDTH-1){
        score+=1000;
        if(p.y()==0||p.y()==WIDTH-1){
            score*=100;
        }
    }
    if(p.y()==0||p.y()==WIDTH-1){
        score+=1000;
    }
    //边角对于黑白棋有重要意义
    int i = p.x();
    int j = p.y();
    int k = i;
    int m = j;
    int color = chesses[i][j];
    int count = 0;
//    QVector<QPoint>* can_point = new QVector<QPoint>();
    while (--m>=0&&chesses[k][m]==-color) {
        count++;
    }
    if(m<0||chesses[k][m]!=color||!count)
        ;
    else {
        while (++m != j) {
            score+=10;
        }
    }
    k=i;
    m=j;
    count=0;
    while (++m<WIDTH&&m<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(m>=WIDTH||chesses[k][m]!=color||!count)
        ;
    else {
        while (--m != j) {
            score+=10;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--k>=0&&k>=0&&chesses[k][m]==-color) {
        count++;
    }
    if(k<0||chesses[k][m]!=color||!count)
        ;
    else {
        while (++k != i) {
            score+=10;
        }
    }
    k=i;
    m=j;
    count=0;
    while (++k<WIDTH&&k<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(k>=WIDTH||chesses[k][m]!=color||!count)
        ;
    else {
        while (--k != i) {
            score+=10;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--k>=0&&--m>=0&&k>=0&&m>=0&&chesses[k][m]==-color) {
        count++;
    }
    if(k<0||m<0||chesses[k][m]!=color||!count)
        ;
    else {
        while (++k != i&&++m != j) {
            score+=10;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--k>=0&&++m<WIDTH&&k>=0&&m<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(k<0||m>=WIDTH||chesses[k][m]!=color||!count)
        ;
    else {
        while (++k != i&& --m != j) {
            score+=10;
        }
    }
    k=i;
    m=j;
    count=0;
    while (--m>=0&&++k<WIDTH&&m>=0&&k<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(m<0||k>=WIDTH||chesses[k][m]!=color||!count)
        ;
    else {
        while (++m != j&& --k != i) {
            score+=10;
        }
    }
    k=i;
    m=j;
    count=0;
    while (++m<WIDTH&&++k<WIDTH&&m<WIDTH&&k<WIDTH&&chesses[k][m]==-color) {
        count++;
    }
    if(m>=WIDTH||k>=WIDTH||chesses[k][m]!=color||!count)
        ;
    else {
        while (--k != i&& --m != j) {
            score+=10;
        }
    }
    return score;
}

void MainBoardWindow::update_time(){
    QString sss;
    if (nowrole == BLACK){
        sss = "BLACK: ";
    }else {
        sss = "WHITE: ";
    }
    sss.append(QString("%1 ").arg(time_count));
    sss.append('s');
//    qDebug()<<sss;
    now_role->setText(sss);
}

void MainBoardWindow::update_board(int a){
    if(gaming){
        int x = a/10;
        int y = a%10;
        chesses[x][y] = -h_role;
        nowrole = -nowrole;
        putting_chess = false;
        clickable = true;
        reverse(x,y);
        repaint();
    }
}
