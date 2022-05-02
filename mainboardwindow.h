#ifndef MAINBOARDWINDOW_H
#define MAINBOARDWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QVector>
#include <QThread>
#include <QLabel>
#include <QMouseEvent>
#define BLACK -1
#define WHITE 1
#define NO_CHESS 0
#define WIDTH 8
namespace Ui {
class MainBoardWindow;
class MyThread;
}
class MainBoardWindow;
class MyThread : public QThread {
    Q_OBJECT
public:
    MainBoardWindow* parent;
    explicit MyThread ( MainBoardWindow *parent = 0 );
    void stop();
protected:
    void run();
private:
    volatile bool stopped;
signals:
public slots:
};

class MainBoardWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool clickable;
    bool gaming;
    QLabel* label;
    QVector<QPoint>* valid_pos;
    QVector<QPoint>* valid_pos_player;
    MyThread* mythread;
    int h_role;
    int nowrole;
    int Level;
    explicit MainBoardWindow(QWidget *parent = nullptr);
    explicit MainBoardWindow(QWidget *parent = nullptr,int level=0);
    void check_valid_pos(int color);
    void check_valid_pos_player(int color);
    void startgame();
    void endgame();
    ~MainBoardWindow();
    void init(int level=0);
    void putchess(int color);
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainBoardWindow *ui;
    int chesses[8][8];
    int mycolor;
    int level;

};

#endif // MAINBOARDWINDOW_H
