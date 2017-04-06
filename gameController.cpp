#include "gameController.h"
#include "ui_gameController.h"
#include<iostream>
#include<string>
#include<queue>
#include<QLabel>
#include<QGridLayout>
#include<QTime>
#include<QKeyEvent>
#include<QMessageBox>
#include<QPainter>
#include<QDebug>

GameController::GameController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameController)
{
    ui->setupUi(this);

    moveTimer = startTimer(INTERVALS);       //初始化计时器 1000 ms/次 触发定时器

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    isStart = true;

    resize(BLOCK_SIZE*width, BLOCK_SIZE*height);   //重定义界面大小

    // new Block() 对象
    for(int row=0;row<height;row++)
        for(int col=0;col<width;col++)
            block[row][col] = new Block();
    // new Snake() 对象
    snake = new Snake();

    initMap();  //初始化地图
    initSnake();//初始化蛇身
    randGenFood();//随机生成食物
}

GameController::~GameController() //释放堆内存中的对象
{
    delete ui;
    for(int row=0;row<height;row++)
        for(int col=0;col<width;col++)
            delete block[row][col];
}

void GameController::initMap()   //初始化地图块
{
    for(int row=0;row<height;row++)
    {
        for(int col=0;col<width;col++)
        {
            //重置 QLabel 的样式及位置
            block[row][col]->type = BlockType::NORMAL_TYPE;
            block[row][col]->x = row;
            block[row][col]->y = col;
            block[row][col]->mrk = -1;  //表示蛇首尚未进过该位置
            //block[row][col]->block->setStyleSheet("QLabel{ background-color: black;}");
//            block[row][col]->block->setGeometry(col*BLOCK_SIZE, row*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
//            label->setNum(row*width+col); //TO TEST  
        }
    }
}

void GameController::initSnake()
{
    if(snake != nullptr)    //清除上一轮游戏中的对象（蛇），防止内存溢出
        delete snake;
    //随机产生长为 1 的蛇，蛇首坐标 (snakeHeadX, snakeHeadY), 蛇首朝向 snakeHeadDir 。
    snake = new Snake();
    snake->length = 1;
    snake->headX = qrand() % width;
    snake->headY = qrand() % height;
    snake->headDir = qrand() % 4;
    snake->headMrk = getIdx(snake->headMrk);
    block[snake->headX][snake->headY]->type = BlockType::SNAKE_TYPE;
    block[snake->headX][snake->headY]->mrk = snake->headMrk;    //标记初始蛇首位置的计数值为 0 .
    snake->snake.push_front(std::make_pair(snake->headX, snake->headY));
//    if(snake->headDir == DIR::UP || snake->headDir == DIR::DOWN)
//        block[snake->headX][snake->headY]->block->setStyleSheet("background-color: white; border: 5px solid black;");
//    else
//        block[snake->headX][snake->headY]->block->setStyleSheet("background-color: white; border: 10px solid black");
}

void GameController::randGenFood()
{
    int x, y;
    do {
        x = qrand() % width;
        y = qrand() % height;
    } while(block[x][y]->type != BlockType::NORMAL_TYPE);
    block[x][y]->type = BlockType::FOOD_TYPE;
    food = block[x][y];
    //    block[x][y]->block->setStyleSheet("background: red; border: 5px solid black;");
}

QRectF GameController::genSnakeRect(std::pair<int, int> preCoordinate, std::pair<int, int> coordinate)
{
    if(preCoordinate.first == -1)
    {
        return QRectF((coordinate.second+0.25)*BLOCK_SIZE, (coordinate.first+0.25)*BLOCK_SIZE,
                      0.5*BLOCK_SIZE, 0.5*BLOCK_SIZE);
    }
    else
    {
        //preCoordinate (row, col+1)  coordinate (row, col);
        if(preCoordinate.first == coordinate.first && preCoordinate.second == coordinate.second+1)
        {
            return QRectF((coordinate.second+0.25)*BLOCK_SIZE, (coordinate.first+0.25)*BLOCK_SIZE,
                          BLOCK_SIZE, 0.5*BLOCK_SIZE);
        }
        //preCoordinate (row, col+1)  coordinate (row, col);
        else if(preCoordinate.first == coordinate.first && preCoordinate.second == coordinate.second-1)
        {
            return QRectF((coordinate.second-0.25)*BLOCK_SIZE, (coordinate.first+0.25)*BLOCK_SIZE,
                          BLOCK_SIZE, 0.5*BLOCK_SIZE);
        }
        //preCoordinate (row+1, col)  coordinate (row, col);
        else if(preCoordinate.second == preCoordinate.second && preCoordinate.first == coordinate.first+1)
        {
            return QRectF((coordinate.second+0.25)*BLOCK_SIZE, (coordinate.first+0.25)*BLOCK_SIZE,
                          0.5*BLOCK_SIZE, BLOCK_SIZE);
        }
        //preCoordinate (row-1, col)  coordinate (row, col);
        else if(preCoordinate.second == preCoordinate.second && preCoordinate.first == coordinate.first-1)
        {
            return QRectF((coordinate.second+0.25)*BLOCK_SIZE, (coordinate.first-0.25)*BLOCK_SIZE,
                          0.5*BLOCK_SIZE, BLOCK_SIZE);
        }
    }
}

bool GameController::isValidPos(int x, int y, bool isVir)
{
    if(x<0 || x>=height || y<0 || y>=width)
        return false;
    if(isVir && vis[x][y])
        return false;
    return true;
}

void GameController::snakeMove(int x, int y)
{
    if(block[x][y]->type == BlockType::NORMAL_TYPE)
    {
        //蛇首位置更新一格
        snake->snake.push_front(std::make_pair(x, y));
        snake->headX = x;
        snake->headY = y;
        snake->headMrk = getIdx(snake->headMrk);
        block[x][y]->type = BlockType::SNAKE_TYPE;
        block[x][y]->mrk = snake->headMrk;

        //删除蛇尾 BLOCK
        std::pair<int, int> tail = snake->snake.back();
        snake->snake.pop_back();
        block[tail.first][tail.second]->type = BlockType::NORMAL_TYPE;
    }
    else if(block[x][y]->type == BlockType::FOOD_TYPE)
    {
        //获得食物，食物块加入蛇身，作为新的蛇首
        snake->snake.push_front(std::make_pair(x, y));
        snake->headX = x;
        snake->headY = y;
        snake->headMrk = getIdx(snake->headMrk);
        block[x][y]->type = BlockType::SNAKE_TYPE;
        block[x][y]->mrk = snake->headMrk;

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
    isStart = false;
    QMessageBox::warning(this, tr("Warning"), tr("Game is Over. You lose!"), QMessageBox::Yes);
}

void GameController::AI()
{
    int minDis = INF, curDis;
    int Dir;
    for(int i=0;i<4;i++)
    {
        if(i+snake->headDir == 1 || i+snake->headDir == 5)
            continue;
        curDis = AI_AStar(i);
        if(curDis < minDis)
            Dir = i,    minDis = curDis;
    }
    //当不存在 策略1 可达的路径时
    if(minDis == INF)
    {
        for(int i=0;i<4;i++)
        {
            if(i+snake->headDir == 1 || i+snake->headDir == 5)
                continue;
        }
    }
    snake->headDir = Dir;
}

void GameController::AI_normal()
{
    if(snake->headY == 0)
    {
        snake->headDir = DIR::UP;
//        turnUp();
        if(snake->headX == 0)
            snake->headDir = DIR::RIGHT;
//            turnRight();
    }
    else if(snake->headX == height-1 && snake->headY == 1)
        snake->headDir = DIR::LEFT;
    else if(snake->headY == 1)
    {
        snake->headDir = snake->headX % 2 ? DIR::DOWN : DIR::RIGHT;
//        snake->headX % 2 ? turnDown() : turnRight();
    }
    else if(snake->headY == width-1)
    {
        snake->headDir = snake->headX % 2 ? DIR::LEFT : DIR::DOWN;
//        snake->headX % 2 ? turnLeft() : turnDown();
    }
    else
    {
        snake->headDir = snake->headX % 2 ? DIR::LEFT : DIR::RIGHT;
    }
}

//判断蛇首四个方位 哪个离食物更近
int GameController::AI_AStar(int headDir)
{
    VirSnake vCurPos, vNxtPos;
    vCurPos.x = snake->headX + DirChg[headDir][0];
    vCurPos.y = snake->headY + DirChg[headDir][1];
    vCurPos.curStp = 1;
    vCurPos.expStp = vCurPos.calEuclidDis(food->x, food->y);

    memset(vis, 0, sizeof(vis));
    for(auto coordinate : snake->snake)
        vis[coordinate.first][coordinate.second] = 1;

    if(!hasWayToTail(vCurPos.x, vCurPos.y))
        return INF;

    std::priority_queue<VirSnake> que;
    que.push(vCurPos);
    while(!que.empty())
    {
        vCurPos = que.top();
        que.pop();
        if(vis[vCurPos.x][vCurPos.y])   continue;
        vis[vCurPos.x][vCurPos.y] = 1;

        if(vCurPos.x == food->x && vCurPos.y == food->y)
            return vCurPos.curStp;

        vNxtPos.curStp = vCurPos.curStp + 1;
        for(int i=0;i<4;i++)
        {
            vNxtPos.x = vCurPos.x + DirChg[i][0];
            vNxtPos.y = vCurPos.y + DirChg[i][1];
            if(!isValidPos(vNxtPos.x, vNxtPos.y, true))   continue;
            vNxtPos.expStp = vNxtPos.calEuclidDis(food->x, food->y);
            que.push(vNxtPos);
        }
    }
    return INF;
}

//获得食物后是否能够有路径抵达蛇尾（防止进入死路）
bool GameController::hasWayToTail(int headX, int headY)
{
    return true;
}

int GameController::getIdx(int oriIdx)
{
    return ++oriIdx == MAX_IDX ? 0 : oriIdx;
}

int GameController::subIdx(int oriIdx, int curIdx)
{
    return curIdx-=oriIdx >= 0 ? curIdx : curIdx + MAX_IDX;
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

    //暂停
    case Qt::Key_P:
        isStart = !isStart;
        if(isStart)
            moveTimer = startTimer(INTERVALS);
        else
            killTimer(moveTimer);
       break;

    //非法键盘事件
    default:
        break;
    }
}

void GameController::timerEvent(QTimerEvent *e)
{
    if(isStart && e->timerId() == moveTimer)   //定时器触发
    {
        //调用贪吃蛇 AI() 函数决定蛇首是否转向
        AI();

        //根据蛇首位置及蛇首方向产生下一步蛇首位置
        snake->headX = snake->headX + DirChg[snake->headDir][0];
        snake->headY = snake->headY + DirChg[snake->headDir][1];

        //蛇移动到下一个位置
        if(!isValidPos(snake->headX, snake->headY, false))
            showErrorMessage();
        else
            snakeMove(snake->headX, snake->headY);

        //重绘 游戏界面
        repaint();
    }
}

void GameController::mousePressEvent(QMouseEvent *)
{
//    //响应鼠标点击事件
//    if(isStart == false)    //如果游戏尚未开始
//    {
//        isStart = true; //设置游戏状态为 游戏中...
//        initMap();      //重置地图
//        initSnake();    //重置蛇
//        randGenFood();  //重置食物
    //    }
}

void GameController::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //绘制背景 全黑
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, BLOCK_SIZE*height, BLOCK_SIZE*width);

    //绘制食物 红点
    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    painter.drawRect((food->y + 0.25)*BLOCK_SIZE, (food->x + 0.25)*BLOCK_SIZE,
                     0.5*BLOCK_SIZE, 0.5*BLOCK_SIZE);

    //绘制蛇 全白
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    std::pair<int, int> preCoordinate = std::make_pair(-1, -1);
    for(auto coordinate : snake->snake)
    {
        painter.drawRect(genSnakeRect(preCoordinate, coordinate));
        preCoordinate = coordinate;
    }
}

void GameController::on_actionNew_Game_N_triggered()    //新的游戏
{
    isStart = false;
    initMap();  //初始化地图
    initSnake();//初始化蛇身
    randGenFood();//随机生成食物
    ui->actionStart_S->setCheckable(true);
}

void GameController::on_actionStart_S_triggered()
{
    //TODO: 设置 Action 不可点击（禁制显示）
    ui->actionStart_S->setCheckable(false);
    isStart = true;
}

void GameController::on_actionExit_Q_triggered()
{
    exit(true);
}

void GameController::on_actionPause_Continue_triggered()
{
    //TODO: 添加实际 暂停/继续 操作逻辑
    QIcon pause(":/icon/assets/Pause_104px.png");
    QIcon contin(":/icon/assets/Resume_Button_96px.png");
    if(ui->actionPause_Continue->text() == tr("Pause(&P)"))
    {
        ui->actionPause_Continue->setIcon(contin);
        ui->actionPause_Continue->setText(tr("Continue(&C)"));
    }
    else
    {
        ui->actionPause_Continue->setIcon(pause);
        ui->actionPause_Continue->setText(tr("Pause(&P)"));
    }
}
