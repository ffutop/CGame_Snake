#ifndef SNAKE_H
#define SNAKE_H

#include<list>

class Block;

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
    int headX, headY;   //蛇头坐标
    int headDir;        //蛇头方向

    std::list<Block*> snake; //蛇身的每一节（按序记录）
};

#endif // SNAKE_H
