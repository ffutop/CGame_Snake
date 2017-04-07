#include "snake.h"
#include<cmath>

Snake::Snake()
{
}

Snake::~Snake()
{
}

VirSnake::VirSnake()
{

}

VirSnake::VirSnake(int _x, int _y)
{
    x = _x, y = _y;
}

VirSnake::VirSnake(int _x, int _y, int _curStp)
{
    x = _x, y = _y;
    curStp = _curStp;
}

VirSnake::VirSnake(int _x, int _y, int _curStp, int _expStp)
{
    x = _x, y = _y;
    curStp = _curStp;
    expStp = _expStp;
}

VirSnake::~VirSnake()
{

}

int VirSnake::calEuclidDis(int aimX, int aimY)
{
    return std::abs(x - aimX) + std::abs(y - aimY);
}
