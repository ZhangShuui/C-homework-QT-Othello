#ifndef MAINBOARDWINDOW_H
#define MAINBOARDWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainBoardWindow;
}

class MainBoardWindow : public QMainWindow
{
    Q_OBJECT

public:
    int Level;
    explicit MainBoardWindow(QWidget *parent = nullptr);
    explicit MainBoardWindow(QWidget *parent = nullptr,int level=0);
    ~MainBoardWindow();

private:
    Ui::MainBoardWindow *ui;
};

#endif // MAINBOARDWINDOW_H
