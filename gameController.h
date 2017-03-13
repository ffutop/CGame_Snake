#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QMainWindow>
#include "block.h"
#include "snake.h"

const int MAX_WIDTH = 20;   //游戏界面最大宽度
const int MAX_HEIGHT = 20;  //游戏界面最大高度
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

    void snakeMove();
    void turnUp();
    void turnDown();
    void turnLeft();
    void turnRight();

protected:
    void keyPressEvent(QKeyEvent *);
    void timerEvent(QTimerEvent *);

private:
    Ui::GameController *ui;
    Block *block[MAX_WIDTH][MAX_HEIGHT];    //游戏界面 二维地图块
    int width = MAX_WIDTH, height=MAX_HEIGHT;  //地图块 宽度与高度
    Snake *snake;   //蛇对象的引用
    int moveTimer;
};

#endif // GAMECONTROLLER_H
