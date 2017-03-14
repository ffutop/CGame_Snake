#include "gameController.h"
#include "ui_gameController.h"
#include<QLabel>
#include<QGridLayout>
#include<QTime>
#include<QKeyEvent>
#include<QMessageBox>

GameController::GameController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameController)
{
    ui->setupUi(this);

    moveTimer = startTimer(500);       //初始化计时器 1000 ms/次 触发定时器
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    resize(BLOCK_SIZE*width, BLOCK_SIZE*height);    //重定义界面大小
    initMap();  //初始化地图
    initSnake();
    randGenFood();

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
//            label->setNum(row*width+col); //TO TEST

            block[row][col]->block = label;     // Block 对象记录 QLabel 的地址，用于后续访问
        }
    }
}

void GameController::initSnake()
{
    snake = new Snake();
    snake->length = 1;
    snake->headX = qrand() % MAX_WIDTH;
    snake->headY = qrand() % MAX_HEIGHT;
    block[snake->headX][snake->headY]->type = BlockType::SNAKE_TYPE;
    block[snake->headX][snake->headY]->block->setStyleSheet("QLabel { background: white }");
    snake->headDir = qrand() % 4;
    snake->snake.push_front(std::make_pair(snake->headX, snake->headY));
}

void GameController::randGenFood()
{
    int x = qrand() % MAX_WIDTH;
    int y = qrand() % MAX_HEIGHT;
    while(block[x][y]->type != BlockType::NORMAL_TYPE)
    {
        x = qrand() % MAX_WIDTH;
        y = qrand() % MAX_HEIGHT;
    }
    block[x][y]->type = BlockType::FOOD_TYPE;
    block[x][y]->block->setStyleSheet("QLabel{ background: white;}");
}

void GameController::snakeMove(int x, int y)
{
    if(block[x][y]->type == BlockType::NORMAL_TYPE)
    {
        //蛇首位置更新一格
        snake->snake.push_front(std::make_pair(x, y));
        snake->headX = x;
        snake->headY = y;
        block[x][y]->type = BlockType::SNAKE_TYPE;
        block[x][y]->block->setStyleSheet("QLabel {background: white;}");

        //删除蛇尾 BLOCK
        std::pair<int, int> tail = snake->snake.back();
        snake->snake.pop_back();
        block[tail.first][tail.second]->block->setStyleSheet("QLabel {background: black;}");
        block[tail.first][tail.second]->type = BlockType::NORMAL_TYPE;
    }
    else if(block[x][y]->type == BlockType::FOOD_TYPE)
    {
        //获得食物，食物块加入蛇身，作为新的蛇首
        snake->snake.push_front(std::make_pair(x, y));
        snake->headX = x;
        snake->headY = y;
        block[x][y]->block->setStyleSheet("QLabel {background: white;}");
        block[x][y]->type = BlockType::SNAKE_TYPE;
        snake->length++;

        //蛇尾不需进行操作

        //生成新的食物
        randGenFood();
    }
    else if(block[x][y]->type == BlockType::SNAKE_TYPE)
    {
        //蛇首碰撞蛇身，游戏结束
        showErrorMessage();
    }
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

void GameController::showErrorMessage()
{
    QMessageBox::warning(this, tr("游戏结束"), tr("You lose!"));
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
        switch (snake->headDir) {   //判断当前蛇首方向
        case DIR::UP:
            if(snake->headX == 0)
                showErrorMessage();
            else
                snakeMove(snake->headX-1, snake->headY);
            break;
        case DIR::DOWN:
            if(snake->headX == height)
                showErrorMessage();
            else
                snakeMove(snake->headX+1, snake->headY);
            break;
        case DIR::LEFT:
            if(snake->headY == 0)
                showErrorMessage();
            else
                snakeMove(snake->headX, snake->headY-1);
            break;
        case DIR::RIGHT:
            if(snake->headY == width)
                showErrorMessage();
            else
                snakeMove(snake->headX, snake->headY+1);
            break;
        default:
            break;
        }
    }
}
