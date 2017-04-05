#ifndef SNAKE_H
#define SNAKE_H

#include<list>

enum DIR {
    UP,     //向上
    DOWN,   //向下
    LEFT,   //向左
    RIGHT   //向右
};

class Snake
{
public:
    Snake();
    ~Snake();
    int length;         //蛇身长度
    int headX, headY;   //蛇首坐标
    int headDir;        //蛇首方向
    int headMrk;        //蛇首计数

    std::list<std::pair<int, int> > snake; //蛇身的每一节（按序记录）
};

#endif // SNAKE_H
