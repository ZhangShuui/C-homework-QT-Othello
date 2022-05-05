#ifndef MAINBOARDWINDOW_H
#define MAINBOARDWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QVector>
#include <QThread>
#include <QLabel>
#include <QMouseEvent>
#include "mcts_pyqt.h"
#include "QSoundEffect"
#define BLACK 1
#define WHITE -1
#define NO_CHESS 0
#define WIDTH 8
#define DEFAULT 99



namespace Ui {
class MainBoardWindow;
class MyThread;
class TimeThread;
class AiThread;
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
    void rop();
    void repp();


public slots:
};
class TimeThread : public QThread {
    Q_OBJECT
public:
    MainBoardWindow* parent;
    explicit TimeThread ( MainBoardWindow *parent = 0 );
    void stop();
protected:
    void run();
private:
    volatile bool stopped;
signals:
    void record();


public slots:
};
class AiThread : public QThread {
    Q_OBJECT
public:
    MainBoardWindow* parent;
    explicit AiThread ( MainBoardWindow *parent = 0 );
    void stop();
protected:
    void run();
private:
    volatile bool stopped;
signals:
    void reeee(int a);


public slots:
};
class SoundThread : public QThread {
    Q_OBJECT
public:
    MainBoardWindow* parent;
    explicit SoundThread ( MainBoardWindow *parent = 0 );
    void stop();
protected:
    void run();
private:
    volatile bool stopped;
signals:
    void reeee(int a);


public slots:
};
class MainBoardWindow : public QMainWindow
{
    Q_OBJECT

public:
    int time_count;
    bool putting_chess;
    bool clickable;
    bool gaming;
    bool playing;
    QSoundEffect *effect;
    QLabel* label;
    QLabel* now_role;
    QLabel* time_label;
    int evaluate(QPoint pp);
    QVector<QPoint>* valid_pos;
    QVector<QPoint>* valid_pos_player;
    QVector<QPoint>* valid_pos_robot;
    QVector<QPoint>* valid_pos_paint;
    TimeThread* timethread;
    AiThread* aithread;
    SoundThread* sthread;
    MyThread* mythread;
    int h_role;
    int nowrole;
    int Level;
    MCTS_pyqt* Hard_AI;
    explicit MainBoardWindow(QWidget *parent = nullptr);
    explicit MainBoardWindow(QWidget *parent = nullptr,int level=0);
    void check_valid_pos(int who ,int color);
//    void check_valid_pos_player(int color);
//    void check_valid_pos_robot(int color);
    void update_valid_pos(int who);
    void reverse(int x,int y);
    void startgame();
    int chesses[8][8];
    ~MainBoardWindow();
    void init(int level=0);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event);
public slots:
    void on_pushButton_clicked();
    void putchess();
    void on_pushButton_3_clicked();
    void endgame();
    void on_pushButton_2_clicked();
    void update_time();
    void on_pushButton_4_clicked();
    void update_board(int );
private:
    Ui::MainBoardWindow *ui;

    int mycolor;
    int level;

};

#endif // MAINBOARDWINDOW_H
