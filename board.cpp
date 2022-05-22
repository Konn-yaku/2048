#include "board.h"

Board::Board()
{
    memset(dig, 0, sizeof(dig));
    digcnt = 0;
}

Board::Board(const Board& other) {
    memcpy(dig, other.dig, 16 * sizeof(char));
    digcnt = other.digcnt;
}

Board& Board::operator=(const Board &other) {
    memcpy(dig, other.dig, 16 * sizeof(char));
    digcnt = other.digcnt;
}

void Board::init() {
    *this = Board();
}

int Board::getDigit(int x, int y) {
    return dig[x][y];
}

int Board::getDigit(int index) {
    return dig[index % 4][index / 4];
}

QPair<char, char> Board::addDigit() {
    QPair<int, int> pos;
    do {
        pos = Random::randpair();
    } while (dig[pos.first][pos.second] != 0);
    digcnt++;
    dig[pos.first][pos.second] = []() {
        char probability = Random::randint(1, 100);
        if (probability <= 10) {
            return 2;
        } else {
            return 1;
        }
    }(); // 有20%的概率生成4,80%概率生成2
    return qMakePair(pos.first * 4 + pos.second, dig[pos.first][pos.second]);
}

bool Board::isDefeated()
{
    if (digcnt != 16) return false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
        {
            if (i != 3 &&
                dig[i][j] == dig[i + 1][j])
                return false;
            if (j != 3 &&
                dig[i][j] == dig[i][j + 1])
                return false;
        }
    }
    return true;
}

bool Board::isWin() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (dig[i][j] == 11) {
                return true;
            }
        }
    }
    return false;
}

QList<QPair<char, char>> Board::operate(Dir dir) {
    switch (dir) {
    case NORTH:
        return base_OperateNorth();
    case EAST:
        return base_OperateEast();
    case SOUTH:
        return base_OperateSouth();
    case WEST:
        return base_OperateWest();
    default:
        return QList<QPair<char, char>>();
    }
}

QList<QPair<char, char>> Board::base_OperateNorth() {
    QList<QPair<char, char>> res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (dig[j][i] == 0) continue;
            for (int k = j + 1; k < 4; k++) {
                if (dig[k][i] == 0) continue;
                if (dig[j][i] == dig[k][i]) {
                    res.append(qMakePair(4 * k + i, -1));
                    res.append(qMakePair(4 * j + i, ++dig[j][i]));
                    dig[k][i] = 0;
                    digcnt--;
                    break;
                } else {
                    break;
                }
            }
        }
        for (int j = 1; j < 4; j++) {
            if (dig[j][i] == 0) continue;
            int ptr = j;
            while (ptr - 1 >= 0 && dig[ptr - 1][i] == 0) ptr--;
            if (ptr != j) {
                res.append(qMakePair(4 * j + i, 4 * ptr + i));
                dig[ptr][i] = dig[j][i];
                dig[j][i] = 0;
            }
        }
    }
    return res;
}

QList<QPair<char, char>> Board::base_OperateEast() {
    QList<QPair<char, char>> res;
    for (int i = 0; i < 4; i++) {
        for (int j = 3; j > 0; j--) {
            if (dig[i][j] == 0) continue;
            for (int k = j - 1; k >= 0; k--) {
                if (dig[i][k] == 0) continue;
                if (dig[i][j] == dig[i][k]) {
                    res.append(qMakePair(4 * i + k, -1));
                    res.append(qMakePair(4 * i + j, ++dig[i][j]));
                    dig[i][k] = 0;
                    digcnt--;
                    break;
                } else {
                    break;
                }
            }
        }
        for (int j = 2; j >= 0; j--) {
            if (dig[i][j] == 0) continue;
            int ptr = j;
            while (ptr + 1 < 4 && this->dig[i][ptr + 1] == 0) ptr++;
            if (ptr != j) {
                res.append(qMakePair(4 * i + j, 4 * i + ptr));
                dig[i][ptr] = dig[i][j];
                dig[i][j] = 0;
            }
        }
    }
    return res;
}

QList<QPair<char, char>> Board::base_OperateSouth()
{
    QList<QPair<char, char>> res;
    for (int i = 0; i < 4; i++) {
        for (int j = 3; j > 0; j--) {
            if (dig[j][i] == 0) continue;
            for (int k = j - 1; k >= 0; k--) {
                if (dig[k][i] == 0) continue;
                if (dig[j][i] == dig[k][i])
                {
                    res.append(qMakePair(4 * k + i, -1));
                    res.append(qMakePair(4 * j + i, ++dig[j][i]));
                    dig[k][i] = 0;
                    digcnt--;
                    break;
                } else {
                    break;
                }
            }
        }
        for (int j = 2; j >= 0; j--) {
            if (dig[j][i] == 0) continue;
            int ptr = j;
            while (ptr + 1 < 4 && dig[ptr + 1][i] == 0) ++ptr;
            if (ptr != j) {
                res.append(qMakePair(4 * j + i, 4 * ptr + i));
                dig[ptr][i] = dig[j][i];
                dig[j][i] = 0;
            }
        }
    }
    return res;
}

QList<QPair<char, char>> Board::base_OperateWest()
{
    QList<QPair<char, char>> res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (dig[i][j] == 0) continue;
            for (int k = j + 1; k < 4; k++) {
                if (dig[i][k] == 0) continue;
                if (dig[i][j] == dig[i][k]) {
                    res.append(qMakePair(4 * i + k, -1));
                    res.append(qMakePair(4 * i + j, ++dig[i][j]));
                    dig[i][k] = 0;
                    digcnt--;
                    break;
                } else {
                    break;
                }
            }
        }
        for (int j = 1; j < 4; j++) {
            if (dig[i][j] == 0) continue;
            int ptr = j;
            while (ptr - 1 >= 0 && this->dig[i][ptr - 1] == 0) ptr--;
            if (ptr != j) {
                res.append(qMakePair(4 * i + j, 4 * i + ptr));
                dig[i][ptr] = dig[i][j];
                dig[i][j] = 0;
            }
        }
    }
    return res;
}
