#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QKeyEvent"
#include "QTimer"
#include "QRect"
#include "register.h"
#include "QQueue"
#include "QStack"
#include "board.h"
#include "QTimer"
#include "digitimg.h"
#include "ranklist.h"
#include <map>
#include "general_algorithm.h"
#include "QPropertyAnimation"
#include "gamehelp.h"
#include "keyhelp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void changeUI_login();
    void changeUI_logout();

    void drawNewDigitImg(QPair<char, char> index_and_val);
    void drawAllDigitImgAfterMove(QList<QPair<char, char>> moves);
    void drawAllDigitImgAfterBack();

    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void on_registerButton_clicked();

    void on_loginButton_clicked();
    void on_newgame_clicked();

    void on_logout_clicked();

    void on_back_clicked();

    void on_rank_clicked();

    void on_keyhelp_clicked();

    void on_gamehelp_clicked();

private:

    Ui::MainWindow *ui;

    Register register_wnd; // 注册窗口
    RankList ranklist_wnd; // 排行榜窗口
    KeyHelp keyhelp_wnd; // 按键说明窗口
    GameHelp gamehelp_wnd; // 游戏说明窗口

    bool is_login; // 是否登录
    bool is_started; // 游戏是否开始
    QString username; // 登录后的用户名

    QLabel* gameover; // 游戏结束的标签
    QTimer* timer; // 计时器
    std::map<int, DigitImg*> digit_imgs;
    QQueue<DigitImg*> delete_queue;

    // 用于回退的栈
    QStack<QPair<Board, QPair<int, int>>> back_stack;

    Board board;
};

#endif // MAINWINDOW_H
