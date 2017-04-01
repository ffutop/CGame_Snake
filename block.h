#ifndef BLOCK_H
#define BLOCK_H

#endif // BLOCK_H

class QLabel;

enum BlockType{     //标签块类型
    NORMAL_TYPE,    //普通标签块
    SNAKE_TYPE,     //蛇身标签块
    FOOD_TYPE       //食物标签块
};

class Block     //标签块类
{
public:
    Block();
    ~Block();
//    QLabel *block;  //使用标签在 GUI 界面显示地图
    int type;       //标签块类型
    int x, y;       //标签块位置 (x, y)
};
