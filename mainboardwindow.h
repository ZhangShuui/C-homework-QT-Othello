#ifndef MAINBOARDWINDOW_H
#define MAINBOARDWINDOW_H

#include <QMainWindow>
#include <list>
#define BLACK -1
#define WHITE 1
#define NO_CHESS 0
#define WIDTH 8
namespace Ui {
class MainBoardWindow;
}

class MainBoardWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool clickable;
    bool gaming;
    int h_role;
    int Level;
    explicit MainBoardWindow(QWidget *parent = nullptr);
    explicit MainBoardWindow(QWidget *parent = nullptr,int level=0);
    ~MainBoardWindow();
    void init(int level=0);
protected:
    void paintEvent(QPaintEvent* event);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainBoardWindow *ui;
    int chesses[8][8];
    int mycolor;
};

#endif // MAINBOARDWINDOW_H
