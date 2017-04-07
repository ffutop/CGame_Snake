#ifndef SNAKE_H
#define SNAKE_H

#include<list>

enum DIR {
    UP,     //向上    (x-1, y);
    DOWN,   //向下    (x+1, y);
    LEFT,   //向左    (x, y-1);
    RIGHT   //向右    (x, y+1);
};

const int DirChg[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

class Snake     //蛇
{
public:
    Snake();
    ~Snake();
    int length;         //蛇身长度
    int headX, headY;   //蛇首坐标
    int headDir;        //蛇首方向

    std::list<std::pair<int, int> > snake; //蛇身的每一节（按序记录）
};

//数据结构 虚拟蛇首
struct VirSnake
{
    VirSnake();
    VirSnake(int _x, int _y);
    VirSnake(int _x, int _y, int _curStp);
    VirSnake(int _x, int _y, int _curStp, int _expStp);
    ~VirSnake();

    int x, y;           // 虚拟蛇首 坐标 (x, y);
    int curStp, expStp; // 当前移动步数   预计所需步数

    //计算虚拟蛇首到达 目标 (aimX, aimY) 的欧几里得距离
    int calEuclidDis(int aimX, int aimY);

    bool operator<(const VirSnake& b) const {
        return curStp+expStp > b.curStp+b.expStp;
    }
};

#endif // SNAKE_H
