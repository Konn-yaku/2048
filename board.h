#ifndef BOARD_H
#define BOARD_H

#include "random.h"
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include <qdebug.h>

enum Dir {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
};

class Board
{
public:
    Board();
    Board(const Board& other);

    int getDigit(int x, int y);
    int getDigit(int index);

    void init();
    QPair<char, char> addDigit(); // .first为序号,.second为数
    QList<QPair<char, char>> operate(Dir dir);
    bool isDefeated();
    bool isWin();

    Board& operator=(const Board& other);


private:
    char digcnt; // 保存棋盘上有几个数
    char dig[4][4]; // 用一个二维数组保存棋盘状态


    // 四种基本可操作
    QList<QPair<char, char>> base_OperateNorth();
    QList<QPair<char, char>> base_OperateEast();
    QList<QPair<char, char>> base_OperateSouth();
    QList<QPair<char, char>> base_OperateWest();
};

#endif // BOARD_H
