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
    init(level);
}

MainBoardWindow::~MainBoardWindow()
{
    delete ui;
}
void MainBoardWindow::init(int lev){
    this->setFixedSize(this->geometry().size());
    gaming = false;
    clickable = false;
    QLabel* label=new QLabel(this);
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
    painter.end();
    painter.begin(this);
    painter.drawPixmap(0,0,My_Back);
    painter.end();
}

void MainBoardWindow::on_pushButton_clicked()
{
    if(gaming)
        return;
    gaming = true;
    int is_black = QMessageBox::question(this,tr(""),tr("作为黑棋先手？"),QMessageBox::Yes,QMessageBox::No);

}
