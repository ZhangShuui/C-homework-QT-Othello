#include "dialog.h"
#include "ui_dialog.h"
#include "mainboardwindow.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    meparent=parent;
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_4_clicked()
{
    this->destroy();
}

void Dialog::on_pushButton_clicked()
{
    MainBoardWindow* uu=new MainBoardWindow(meparent,1);
    uu->show();
    destroy();
}

void Dialog::on_pushButton_2_clicked()
{
    MainBoardWindow* uu=new MainBoardWindow(meparent,2);
    uu->show();
    destroy();
}

void Dialog::on_pushButton_3_clicked()
{
    MainBoardWindow* uu=new MainBoardWindow(meparent,3);
    uu->show();
    destroy();
}
