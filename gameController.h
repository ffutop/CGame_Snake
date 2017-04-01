#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QMainWindow>
#include "block.h"
#include "snake.h"

const int MAX_WIDTH = 18;   //游戏界面最大宽度
const int MAX_HEIGHT = 18;  //游戏界面最大高度
const int BLOCK_SIZE = 35;  //每个标签块的大小（像素）

namespace Ui {
class GameController;
}

class GameController : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameController(QWidget *parent = 0);
    ~GameController();
    void initMap();     //初始化地图
    void initSnake();   //初始化蛇
    void randGenFood(); //随机产生食物块

    QRectF genSnakeRect(std::pair<int, int> coordinate);

    void snakeMove(int x, int y);
    void turnUp();
    void turnDown();
    void turnLeft();
    void turnRight();

    void showErrorMessage();

protected:
    void keyPressEvent(QKeyEvent *e);
    void timerEvent(QTimerEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *);

private slots:
    void on_actionNew_Game_N_triggered();

    void on_actionStart_S_triggered();

    void on_actionExit_Q_triggered();

    void on_actionPause_Continue_triggered();

private:
    Ui::GameController *ui;
    int width = MAX_WIDTH, height=MAX_HEIGHT;  //地图块 宽度与高度
    int moveTimer;
    bool isStart;   //状态量 是否在游戏中
    Block *block[MAX_WIDTH][MAX_HEIGHT];    //游戏界面 二维地图块
    Snake *snake;   //蛇对象的引用
    Block *food;    //记录食物位置
};

#endif // GAMECONTROLLER_H
