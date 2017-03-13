#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include "block.h"

const int MAX_WIDTH = 20;  //游戏界面最大宽度
const int MAX_HEIGHT = 20; //游戏界面最大高度
const int BLOCK_SIZE = 35;

namespace Ui {
class Snake;
}

class Snake : public QMainWindow
{
    Q_OBJECT

public:
    explicit Snake(QWidget *parent = 0);
    ~Snake();
    void initMap();  //初始化地图

private:
    Ui::Snake *ui;
    Block *block[MAX_WIDTH][MAX_HEIGHT];    //游戏界面 二维地图块
    int width = MAX_WIDTH, height=MAX_HEIGHT;  //地图块 宽度与高度

};

#endif // SNAKE_H
