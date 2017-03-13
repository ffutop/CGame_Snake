#include "block.h"
#include<QLabel>

Block::Block()
{
    block = nullptr;
    type = BlockType::NORMAL_TYPE;
    x = y = 0;
}

Block::~Block()
{
    delete block;
}
