#include "snake.h"
#include "ui_snake.h"
#include<QLabel>
#include<QGridLayout>

Snake::Snake(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Snake)
{
    ui->setupUi(this);

    resize(BLOCK_SIZE*width, BLOCK_SIZE*height);    //重定义界面大小
    initMap();

}

Snake::~Snake() //释放堆内存中的对象
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

void Snake::initMap()   //初始化地图块
{
    for(int row=0;row<height;row++)
    {
        for(int col=0;col<width;col++)
        {
            block[row][col] = new Block();      //创建 Block 对象
            QLabel *label = new QLabel(this);   //创建 QLabel 对象

            //重置 QLabel 的样式及位置
            label->setStyleSheet("QLabel{ background-color: gray; border-radius: 10px;}");
            label->setGeometry(col*BLOCK_SIZE, row*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
            label->setNum(row*width+col);

            block[row][col]->block = label;     // Block 对象记录 QLabel 的地址，用于后续访问
        }
    }

}
