#include "gameController.h"
#include "ui_gameController.h"
#include<QLabel>
#include<QGridLayout>
#include<QTime>
#include<QKeyEvent>

GameController::GameController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameController)
{
    ui->setupUi(this);

    moveTimer = startTimer(1000);       //初始化计时器 1000 ms/次 触发定时器

    resize(BLOCK_SIZE*width, BLOCK_SIZE*height);    //重定义界面大小
    initMap();  //初始化地图

}

GameController::~GameController() //释放堆内存中的对象
{
    delete ui;
    for(int row=0;row<height;row++)
    {
        for(int col=0;col<width;col++)
        {
            delete block[row][col];
        }
    }
}

void GameController::initMap()   //初始化地图块
{
    for(int row=0;row<height;row++)
    {
        for(int col=0;col<width;col++)
        {
            block[row][col] = new Block();      //创建 Block 对象
            QLabel *label = new QLabel(this);   //创建 QLabel 对象

            //重置 QLabel 的样式及位置
            label->setStyleSheet("QLabel{ background-color: black;}");
            label->setGeometry(col*BLOCK_SIZE, row*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
            label->setNum(row*width+col);

            block[row][col]->block = label;     // Block 对象记录 QLabel 的地址，用于后续访问
        }
    }
}

void GameController::initSnake()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    snake = new Snake();
    snake->headX = qrand() % MAX_WIDTH;
    snake->headY = qrand() % MAX_HEIGHT;
    snake->headDir = qrand() % 4;
    snake->length = 3;
}

void GameController::randGenFood()
{

}

void GameController::snakeMove()
{

}

void GameController::turnUp()
{
    if(snake->headDir == DIR::DOWN) //转向与当前蛇头方向相反，非法
        return;
    snake->headDir = DIR::UP;
}

void GameController::turnDown()
{
    if(snake->headDir == DIR::UP)   //转向与当前蛇头方向相反，非法
        return;
    snake->headDir = DIR::DOWN;
}

void GameController::turnLeft()
{
    if(snake->headDir == DIR::RIGHT)    //转向与当前蛇头方向相反，非法
        return;
    snake->headDir = DIR::LEFT;
}

void GameController::turnRight()
{
    if(snake->headDir == DIR::LEFT) //转向与当前蛇头方向相反，非法
        return;
    snake->headDir = DIR::RIGHT;
}

void GameController::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {

    //向上转向
    case Qt::Key_Up:
    case Qt::Key_W:
        turnUp();   break;

    //向下转向
    case Qt::Key_Down:
    case Qt::Key_S:
        turnDown(); break;

    //向左转向
    case Qt::Key_Left:
    case Qt::Key_A:
        turnLeft(); break;

    //向右转向
    case Qt::Key_Right:
    case Qt::Key_D:
        turnRight();    break;

    //非法键盘事件
    default:
        break;
    }
}

void GameController::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == moveTimer)   //定时器触发
    {
        snakeMove();    //调用 贪吃蛇 移动函数
    }
}
