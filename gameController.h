#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QMainWindow>
#include "block.h"
#include "snake.h"

const int INF = (1ll<<30) - 1;
const int MAX_WIDTH = 18;   //游戏界面最大宽度  横向 块数
const int MAX_HEIGHT = 18;  //游戏界面最大高度  纵向 块数
const int BLOCK_SIZE = 35;  //每个块的大小（像素）
const int INTERVALS = 30;   //timerEvent 触发的间隔时间
//const int MAX_IDX = 10000000;   //设置最大计数值(idx)，采用循环计数的方式
const std::pair<int, int> ERROR_PAIR = std::make_pair(-1, -1);

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

    /* 绘制 */

    //生成需绘制的蛇身块 （矩形）
    QRectF genSnakeRect(std::pair<int, int> preCoordinate, std::pair<int, int> coordinate);

    /* 蛇身移动 */
    void snakeMove(int x, int y);
    void turnUp();
    void turnDown();
    void turnLeft();
    void turnRight();

    void showErrorMessage();

    /* AI 函数，智能产生蛇的移动策略 */
    //判断移动到位置 (x, y) 是否合法
    bool isValidPos(int x, int y, bool isVir);

    //智能 AI ,自动路径产生
    void AI();

    //按规则进行移动，每次覆盖全图
    void AI_normal();

    // A* 搜索最短距离
    int AI_AStar(int headDir);

    //判断移动到 (headX, headY) 后能够到达蛇尾
    bool hasWayToTail(int headX, int headY);

protected:
    void keyPressEvent(QKeyEvent *e);
    void timerEvent(QTimerEvent *e);
//    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *);

private slots:
//    void on_actionNew_Game_N_triggered();

//    void on_actionStart_S_triggered();

//    void on_actionExit_Q_triggered();

//    void on_actionPause_Continue_triggered();

private:
    Ui::GameController *ui;
    int width = MAX_WIDTH, height=MAX_HEIGHT;  //地图块 宽度与高度
    int moveTimer;  //保存计时器 ID
    bool isStart;   //状态量 是否在游戏中
    Block *block[MAX_HEIGHT][MAX_WIDTH];    //游戏界面 二维地图块
    Snake *snake = nullptr;   //蛇对象的引用
    Block *food;    //记录食物位置

    bool vis[MAX_HEIGHT][MAX_WIDTH];
};

#endif // GAMECONTROLLER_H
