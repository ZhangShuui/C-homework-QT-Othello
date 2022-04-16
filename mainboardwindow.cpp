#include "mainboardwindow.h"
#include "ui_mainboardwindow.h"

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
    init(level);
}

MainBoardWindow::~MainBoardWindow()
{
    delete ui;
}
void MainBoardWindow::init(int lev){

}
