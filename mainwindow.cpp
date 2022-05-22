#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化登录状态
    is_login = false;
    is_started = false;
    username = QString();

    QPixmap* pixmap = nullptr;
    // 设置背景
    QPalette pe = palette();
    QColor color;
    color.setRgb(251,249,239);
    pe.setColor(QPalette::Window, color);
    setPalette(pe);

    // 设置棋盘
    pixmap = new QPixmap(":/img/resources/board.jpg");
    pixmap->scaled(ui->board->size(),Qt::KeepAspectRatio);
    ui->board->setScaledContents(true);
    ui->board->setPixmap(*pixmap);
    delete pixmap;
    pixmap = nullptr;

    // 设置logo
    pixmap = new QPixmap(":/img/resources/logo.jpg");
    pixmap->scaled(ui->logo->size(),Qt::KeepAspectRatio);
    ui->logo->setScaledContents(true);
    ui->logo->setPixmap(*pixmap);
    delete pixmap;
    pixmap = nullptr;

    // 设置当前分数
    pixmap = new QPixmap(":/img/resources/score.jpg");
    pixmap->scaled(ui->score->size(),Qt::KeepAspectRatio);
    ui->score->setScaledContents(true);
    ui->score->setPixmap(*pixmap);
    delete pixmap;
    pixmap = nullptr;

    // 设置最高分数
    pixmap = new QPixmap(":/img/resources/bestscore.jpg");
    pixmap->scaled(ui->score_best->size(),Qt::KeepAspectRatio);
    ui->score_best->setScaledContents(true);
    ui->score_best->setPixmap(*pixmap);
    delete pixmap;
    pixmap = nullptr;

    // 设置用户名
    pixmap = new QPixmap(":/img/resources/username.jpg");
    pixmap->scaled(ui->label->size(),Qt::KeepAspectRatio);
    ui->label->setScaledContents(true);
    ui->label->setPixmap(*pixmap);
    delete pixmap;
    pixmap = nullptr;

    // 设置密码
    pixmap = new QPixmap(":/img/resources/password.jpg");
    pixmap->scaled(ui->label_2->size(),Qt::KeepAspectRatio);
    ui->label_2->setScaledContents(true);
    ui->label_2->setPixmap(*pixmap);
    delete pixmap;
    pixmap = nullptr;

    // 设置提示不可见
    ui->error_msg->setVisible(false);

    // 设置gameover
    gameover = nullptr;
    timer = nullptr;

    // 设置登录后的ui不可用
    ui->step->setVisible(false);
    ui->time->setVisible(false);
    ui->rank->setVisible(false);
    ui->newgame->setVisible(false);
    ui->back->setVisible(false);
    ui->logout->setVisible(false);
    ui->step_cnt->setVisible(false);
    ui->score_cnt->setVisible(false);
    ui->score_max_cnt->setVisible(false);
    ui->time_cnt->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 为新添加的数画图
void MainWindow::drawNewDigitImg(QPair<char, char> index_and_val) {
    DigitImg* di = new DigitImg();
    di->setParent(this);
    di->setIndex(index_and_val.first);
    di->setValue(index_and_val.second);
    di->setGeometry(img_pos[(int)index_and_val.first]);
    char path_buf[50];
    sprintf(path_buf, ":/img/resources/%d.jpg", 1 << index_and_val.second);
    // 设置图片
    QPixmap* pixmap = new QPixmap(path_buf);
    pixmap->scaled(di->size(),Qt::KeepAspectRatio);
    di->setScaledContents(true);
    di->setPixmap(*pixmap);
    delete pixmap;
    pixmap = nullptr;

    // 设置可显示
    di->raise();
    di->setVisible(true);
    digit_imgs[index_and_val.first] = di;

    // 插入动画
    QPropertyAnimation *animation = new QPropertyAnimation(di, "geometry");
    animation->setDuration(200);
    animation->setStartValue(QRect(img_pos[(int)index_and_val.first].left() + 42, img_pos[(int)index_and_val.first].top() + 40, 0, 0));
    animation->setEndValue(img_pos[(int)index_and_val.first]);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

// 移动后的画图
void MainWindow::drawAllDigitImgAfterMove(QList<QPair<char, char> > moves) {
    // 所有进行了合并的要进行一次放大动画
    QQueue<DigitImg*> que;
    for (auto it = moves.begin(); it != moves.end(); it++) {
        // move.first : 移动前的序号,move.second : 若>=0则为移动后的序号,否则-1为被合并的块,-1后的为合并之后的数值
        auto move = *it;
        if (move.second >= 0) {
            // 说明是进行移动,直接修改控件的坐标即可

            // 插入动画
            QPropertyAnimation *animation = new QPropertyAnimation(digit_imgs[move.first], "geometry");
            animation->setDuration(100);
            animation->setStartValue(img_pos[(int)move.first]);
            animation->setEndValue(img_pos[(int)move.second]);
            animation->start(QAbstractAnimation::DeleteWhenStopped);

            // 修改digit_imgs中的值
            digit_imgs[move.second] = digit_imgs[move.first];
            digit_imgs[move.second]->setIndex(move.second);
            digit_imgs.erase(move.first);
        } else {
            // 说明是进行合并,先删除被合并的控件

            // 先插入动画让被删除的空间移至被合并的位置

            QPropertyAnimation *animation = new QPropertyAnimation(digit_imgs[move.first], "geometry");
            animation->setDuration(100);
            animation->setStartValue(img_pos[(int)move.first]);
            animation->setKeyValueAt(0.9, img_pos[(int)(*(it + 1)).first]);
            animation->setEndValue(QRect(img_pos[(int)(*(it + 1)).first].left() + 42, img_pos[(int)(*(it + 1)).first].top() + 40, 0, 0));
            animation->start(QAbstractAnimation::DeleteWhenStopped);

            // 将其设置为不可见

            // 然后将控件加入删除队列
            delete_queue.enqueue(digit_imgs[move.first]);
            digit_imgs.erase(move.first);

            // 再替换合并后的控件的图
            auto move2 = *(++it);
            char path_buf[50];
            sprintf(path_buf, ":/img/resources/%d.jpg", 1 << move2.second);
            QPixmap* pixmap = new QPixmap(path_buf);
            pixmap->scaled(digit_imgs[move2.first]->size(),Qt::KeepAspectRatio);
            digit_imgs[move2.first]->setScaledContents(true);
            digit_imgs[move2.first]->setValue(move2.second);
            digit_imgs[move2.first]->setPixmap(*pixmap);
            delete pixmap;
            pixmap = nullptr;
            que.enqueue(digit_imgs[move2.first]);

            // 分数增加
            int iScore_now = ui->score_cnt->text().toInt();
            ui->score_cnt->setText(QString::number(iScore_now + (1 << move2.second)));

            // 如果最高分比当前分数小的话,分数增加
            int iScore_max = ui->score_max_cnt->text().toInt();
            if (ui->score_cnt->text().toInt() > iScore_max) {
                ui->score_max_cnt->setText(ui->score_cnt->text());
            }
        }
    }

    while (!que.isEmpty()) {
        auto node = que.dequeue();
        auto animation = new QPropertyAnimation(node, "geometry");
        animation->setDuration(200);
        animation->setStartValue(QRect(img_pos[(int)node->getIndex()].left() + 42, img_pos[(int)node->getIndex()].top() + 40, 0, 0));
        animation->setKeyValueAt(0.5, QRect(img_pos[(int)node->getIndex()].left() - 21, img_pos[(int)node->getIndex()].top() - 20, img_pos[(int)node->getIndex()].width() + 42, img_pos[(int)node->getIndex()].height() + 39));
        animation->setEndValue(img_pos[(int)node->getIndex()]);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}


void MainWindow::on_registerButton_clicked()
{
    this->register_wnd.show();
}

void MainWindow::on_loginButton_clicked()
{
    // 获取用户名,密码
    QString username_contain = this->ui->username->text();
    QString password_contain = this->ui->password->text();

    // 如果用户名为空,提示输入用户名
    if (username_contain.isEmpty()) {
        this->ui->error_msg->setText("请输入用户名");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        this->ui->error_msg->setPalette(pe);
        this->ui->error_msg->setVisible(true);
        return;
    }

    // 如果密码为空,提示输入密码
    if (password_contain.isEmpty()) {
        this->ui->error_msg->setText("请输入密码");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        this->ui->error_msg->setPalette(pe);
        this->ui->error_msg->setVisible(true);
        return;
    }

    // 连接数据库
    auto sql_opt = SqlOperator::getInstance();
    if (!sql_opt->connect()) {
        qDebug() << "failed to connect";
    }

    // 如果表不存在则创建表
    QSqlQuery* sql_result;
    if (!sql_opt->isTableExist("Users")) {
        sql_result = sql_opt->operate("CREATE TABLE Users("
                                      "username TEXT PRIMARY KEY NOT NULL, "
                                      "password TEXT NOT NULL);");
        if (nullptr == sql_result) {
            qDebug() << "Error, create table failed!";
        } else {
            sql_result->finish();
        }
    }

    // 在表中查询username是否已经存在
    sql_result = sql_opt->operate(QString("SELECT COUNT(*) FROM `Users` WHERE `username` = \'") + username_contain + QString("\';"));
    if (nullptr == sql_result) {
        qDebug() << "Error, query failed!";
    }
    sql_result->next();
    if (sql_result->value(0).toInt() == 0) {
        this->ui->error_msg->setText("用户不存在");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        this->ui->error_msg->setPalette(pe);
        this->ui->error_msg->setVisible(true);
        sql_result->finish();
        sql_opt->close();
        return;
    }
    sql_result->finish();

    // 在表中查询username对应的密码
    sql_result = sql_opt->operate(QString("SELECT `password` FROM `Users` WHERE `username` = \'") + username_contain + QString("\';"));
    if (nullptr == sql_result) {
        qDebug() << "Error, query failed!";
    }
    sql_result->next();
    if (0 != QString::compare(sql_result->value(0).toString(), password_contain)) {
        this->ui->error_msg->setText("密码错误");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        this->ui->error_msg->setPalette(pe);
        this->ui->error_msg->setVisible(true);
        sql_result->finish();
        sql_opt->close();
        return;
    }
    sql_result->finish();
    sql_opt->close();

    // 登录成功,更改UI
    this->changeUI_login();
    this->username = username_contain;

}

int getMaxScoreFromSqlite() {
    auto sql_opt = SqlOperator::getInstance();

    if (!sql_opt->connect()) {
        qDebug() << "failed to connect";
    }

    // 如果表不存在则创建表
    QSqlQuery* sql_result;
    if (!sql_opt->isTableExist("Scores")) {
        sql_result = sql_opt->operate("CREATE TABLE Scores("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                                      "username TEXT NOT NULL, "
                                      "score INTEGER NOT NULL);");
        if (nullptr == sql_result) {
            qDebug() << "Error, create table failed!";
        } else {
            sql_result->finish();
        }
    }

    // 如果表为空,返回0
    sql_result = sql_opt->operate(QString("SELECT COUNT(*) FROM `Scores`;"));
    if (nullptr == sql_result) {
        qDebug() << "Error, query failed!";
    }

    sql_result->next();
    if (0 == sql_result->value(0).toInt()) {
        sql_result->finish();
        sql_opt->close();
        return 0;
    }

    // 在表中查询最高分数
    sql_result = sql_opt->operate(QString("SELECT MAX(`score`) FROM `Scores`;"));
    sql_result->next();
    int max_score = sql_result->value(0).toInt();
    sql_result->finish();
    sql_opt->close();
    return max_score;
}

void MainWindow::changeUI_login() {
    // 隐藏原有控件
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->username->setVisible(false);
    ui->password->setVisible(false);
    ui->loginButton->setVisible(false);
    ui->registerButton->setVisible(false);
    ui->error_msg->setVisible(false);
    ui->gamehelp->setVisible(false);
    ui->keyhelp->setVisible(false);

    // 显示新控件
    QPixmap* pixmap = nullptr;

    // 显示排行榜按钮
    ui->rank->setVisible(true);

    // 显示回退按钮
    ui->back->setVisible(true);

    // 显示新游戏按钮
    ui->newgame->setVisible(true);

    // 显示注销按钮
    ui->logout->setVisible(true);

    // 显示步数计数器
    ui->step_cnt->setVisible(true);

    // 显示当前分计数器
    ui->score_cnt->setVisible(true);

    // 显示最高分计数器
    ui->score_max_cnt->setText(QString::number(getMaxScoreFromSqlite()));
    ui->score_max_cnt->setVisible(true);

    // 显示时间计数器
    ui->time_cnt->setVisible(true);

    // 显示步数
    pixmap = new QPixmap(":/img/resources/step.jpg");
    pixmap->scaled(ui->step->size(),Qt::KeepAspectRatio);
    ui->step->setScaledContents(true);
    ui->step->setPixmap(*pixmap);
    delete pixmap;
    pixmap = nullptr;
    ui->step->setVisible(true);

    // 显示时间
    pixmap = new QPixmap(":/img/resources/time.jpg");
    pixmap->scaled(ui->time->size(),Qt::KeepAspectRatio);
    ui->time->setScaledContents(true);
    ui->time->setPixmap(*pixmap);
    delete pixmap;
    pixmap = nullptr;
    ui->time->setVisible(true);
}

void MainWindow::changeUI_logout() {
    // 显示原有控件
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->username->setVisible(true);
    ui->password->setVisible(true);
    ui->loginButton->setVisible(true);
    ui->registerButton->setVisible(true);
    ui->gamehelp->setVisible(true);
    ui->keyhelp->setVisible(true);

    // 隐藏新控件
    // 隐藏排行榜按钮
    ui->rank->setVisible(false);

    // 隐藏回退按钮
    ui->back->setVisible(false);

    // 隐藏新游戏按钮
    ui->newgame->setVisible(false);

    // 隐藏注销按钮
    ui->logout->setVisible(false);

    // 隐藏步数计数器
    ui->step_cnt->setVisible(false);

    // 隐藏当前分计数器
    ui->score_cnt->setVisible(false);

    // 隐藏最高分计数器
    ui->score_max_cnt->setVisible(false);

    // 隐藏时间计数器
    ui->time_cnt->setVisible(false);

    // 隐藏步数
    ui->step->setVisible(false);

    // 隐藏时间
    ui->time->setVisible(false);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (false == is_started) {
        return;
    }
    // 清除上一次操作前残留的对象
    while (!delete_queue.isEmpty()) {
        auto node = delete_queue.dequeue();
        node->deleteLater();
    }

    QList<QPair<char, char>> moves;
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        moves = board.operate(NORTH);
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        moves = board.operate(SOUTH);
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        moves = board.operate(WEST);
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        moves = board.operate(EAST);
        break;
    }
    // 成功移动,得到新棋盘
    if (!moves.empty()) {
        // 重画棋盘,同时完成加分
        drawAllDigitImgAfterMove(moves);
        drawNewDigitImg(board.addDigit());

        // 计数器+1
        int iStep_cnt = ui->step_cnt->text().toInt();
        ui->step_cnt->setText(QString::number(iStep_cnt + 1));

        // 将新状态入栈
        back_stack.push(qMakePair(board, qMakePair(ui->score_cnt->text().toInt(), ui->score_max_cnt->text().toInt())));
    }
    if (board.isWin()) {
        // 游戏结束
        this->is_started = false;
        this->releaseKeyboard();


        // 清除残留的对象
        while (!delete_queue.isEmpty()) {
            auto node = delete_queue.dequeue();
            node->deleteLater();
        }

        // 清空回退栈
        back_stack.clear();

        // 停止计时器
        timer->stop();
        timer->deleteLater();
        timer = nullptr;

        // 将分数存入数据库
        auto sql_opt = SqlOperator::getInstance();

        if (!sql_opt->connect()) {
            qDebug() << "failed to connect";
        }

        // 如果表不存在则创建表
        QSqlQuery* sql_result;
        if (!sql_opt->isTableExist("Scores")) {
            sql_result = sql_opt->operate("CREATE TABLE Scores("
                                          "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                                          "username TEXT NOT NULL, "
                                          "score INTEGER NOT NULL);");
            if (nullptr == sql_result) {
                qDebug() << "Error, create table failed!";
            } else {
                sql_result->finish();
            }
        }

        sql_result = sql_opt->operate(
                    QString("INSERT INTO `Scores`(`username`, `score`) VALUES (\'") +
                    this->username + QString("\', ") + ui->score_cnt->text() + QString(");"));


        // 显示Gameover
        QFont font;
        font.setFamily("Console");
        font.setBold(true);
        font.setPointSize(28);

        gameover = new QLabel("You Win!", this);
        gameover->setStyleSheet("color: rgb(255, 255, 255);");
        gameover->setFont(font);
        gameover->setAlignment(Qt::AlignHCenter);
        gameover->setVisible(true);
        auto animation = new QPropertyAnimation(gameover, "geometry");
        auto pos = QRect(135,320,271,121);
        animation->setDuration(1000);
        animation->setStartValue(QRect(pos.left() + 135, pos.top() + 60, 0, 0));
        animation->setEndValue(pos);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    if (board.isDefeated()) {
        // 游戏结束
        this->is_started = false;
        this->releaseKeyboard();


        // 清除残留的对象
        while (!delete_queue.isEmpty()) {
            auto node = delete_queue.dequeue();
            node->deleteLater();
        }

        // 清空回退栈
        back_stack.clear();

        // 停止计时器
        timer->stop();
        timer->deleteLater();
        timer = nullptr;

        // 将分数存入数据库
        auto sql_opt = SqlOperator::getInstance();

        if (!sql_opt->connect()) {
            qDebug() << "failed to connect";
        }

        // 如果表不存在则创建表
        QSqlQuery* sql_result;
        if (!sql_opt->isTableExist("Scores")) {
            sql_result = sql_opt->operate("CREATE TABLE Scores("
                                          "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                                          "username TEXT NOT NULL, "
                                          "score INTEGER NOT NULL);");
            if (nullptr == sql_result) {
                qDebug() << "Error, create table failed!";
            } else {
                sql_result->finish();
            }
        }

        sql_result = sql_opt->operate(
                    QString("INSERT INTO `Scores`(`username`, `score`) VALUES (\'") +
                    this->username + QString("\', ") + ui->score_cnt->text() + QString(");"));


        // 显示Gameover
        QFont font;
        font.setFamily("Console");
        font.setBold(true);
        font.setPointSize(28);

        gameover = new QLabel("Game Over!", this);
        gameover->setStyleSheet("color: rgb(255, 255, 255);");
        gameover->setFont(font);
        gameover->setAlignment(Qt::AlignHCenter);
        gameover->setVisible(true);
        auto animation = new QPropertyAnimation(gameover, "geometry");
        auto pos = QRect(135,320,271,121);
        animation->setDuration(1000);
        animation->setStartValue(QRect(pos.left() + 135, pos.top() + 60, 0, 0));
        animation->setEndValue(pos);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QWidget::keyPressEvent(event);
}

void MainWindow::on_newgame_clicked()
{
    if (nullptr != gameover) {
        gameover->deleteLater();
        gameover = nullptr;
    }
    // 先清除所有原有控件
    for (auto it : digit_imgs) {
        it.second->deleteLater();
    }
    digit_imgs.clear();

    // 计数器归零
    ui->step_cnt->setText("0");
    ui->score_cnt->setText("0");
    ui->time_cnt->setText("00:00");

    // 重置计时器
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, [&]() {
        QStringList time_list = ui->time_cnt->text().split(":");
        QTime time;
        if (time_list[1].toInt() == 59) {
            time.setHMS(0, time_list[0].toInt() + 1, 0);
        } else {
            time.setHMS(0, time_list[0].toInt(), time_list[1].toInt() + 1);
        }
        ui->time_cnt->setText(time.toString("mm:ss"));
    });

    timer->start(1000);

    // 开始游戏
    is_started = true;
    this->grabKeyboard();
    board.init();
    drawNewDigitImg(board.addDigit());
    drawNewDigitImg(board.addDigit());

    // 将初始状态入栈
    back_stack.push(qMakePair(board, qMakePair(0, 0)));
}

void MainWindow::on_logout_clicked()
{
    // 确保游戏结束
    this->is_started = false;
    this->releaseKeyboard();

    while (!delete_queue.isEmpty()) {
        auto node = delete_queue.dequeue();
        node->deleteLater();
    }

    if (nullptr != gameover) {
        gameover->deleteLater();
        gameover = nullptr;
    }

    // 清除控件
    for (auto it : digit_imgs) {
        it.second->deleteLater();
    }
    digit_imgs.clear();

    // 清空回退栈
    back_stack.clear();

    // 停止计时器
    if (nullptr != timer) {
        timer->stop();
        timer->deleteLater();
        timer = nullptr;
    }

    // 计数器归零
    ui->step_cnt->setText("0");
    ui->score_cnt->setText("0");
    ui->time_cnt->setText("00:00");

    // 改变ui
    this->changeUI_logout();

    // 清空密码
    ui->password->setText("");
}

void MainWindow::on_back_clicked()
{

    // 游戏未开始或者已经结束,不允许回退
    if (!is_started) return;

    // 游戏处于初始状态,不允许回退
    if (back_stack.size() <= 1) return;

    // 其他情况可以回退
    back_stack.pop();
    auto node = back_stack.top();

    // 棋盘回退
    board = node.first;

    // 计步器减一
    ui->step_cnt->setText(QString::number(ui->step_cnt->text().toInt() - 1));

    // 分数回退
    ui->score_cnt->setText(QString::number(node.second.first));

    // 最高分回退
    ui->score_max_cnt->setText(QString::number(node.second.second));

    // 清空未清空的控件
    while (!delete_queue.isEmpty()) {
        auto node = delete_queue.dequeue();
        node->deleteLater();
    }
    for (auto it : digit_imgs) {
        it.second->deleteLater();
    }
    digit_imgs.clear();

    // 重画界面
    this->drawAllDigitImgAfterBack();
}

void MainWindow::drawAllDigitImgAfterBack() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (0 != board.getDigit(i, j)) {
                auto index_and_val = qMakePair(4 * i + j, board.getDigit(i, j));

                // 开始画图
                DigitImg* di = new DigitImg();
                di->setParent(this);
                di->setIndex(index_and_val.first);
                di->setValue(index_and_val.second);
                di->setGeometry(img_pos[(int)index_and_val.first]);
                char path_buf[50];
                sprintf(path_buf, ":/img/resources/%d.jpg", 1 << index_and_val.second);

                // 设置图片
                QPixmap* pixmap = new QPixmap(path_buf);
                pixmap->scaled(di->size(),Qt::KeepAspectRatio);
                di->setScaledContents(true);
                di->setPixmap(*pixmap);
                delete pixmap;
                pixmap = nullptr;

                // 设置可显示
                di->raise();
                di->setVisible(true);
                digit_imgs[index_and_val.first] = di;
            }
        }
    }
}

void MainWindow::on_rank_clicked()
{

    this->ranklist_wnd.setTable();
    ranklist_wnd.show();
}

void MainWindow::on_gamehelp_clicked()
{
    gamehelp_wnd.show();
}

void MainWindow::on_keyhelp_clicked()
{
    keyhelp_wnd.show();
}
