#ifndef SNAKE_H
#define SNAKE_H

#include<list>

enum DIR {
    UP,     //向上
    DOWN,   //向下
    LEFT,   //向左
    RIGHT   //向右
};

const int DirChg[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

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

struct VirSnake
{
    VirSnake();
    VirSnake(int _x, int _y);
    VirSnake(int _x, int _y, int _curStp);
    VirSnake(int _x, int _y, int _curStp, int _expStp);
    ~VirSnake();
    int x, y, curStp, expStp;

    int calEuclidDis(int foodX, int foodY);

    bool operator<(const VirSnake& b) const {
        return curStp+expStp > b.curStp+b.expStp;
    }
};

#endif // SNAKE_H
