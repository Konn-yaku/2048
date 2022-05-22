#include "register.h"
#include "ui_register.h"
#include <fstream>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    QPixmap* pixmap = nullptr;

    // 设置背景
    QPalette pe = palette();
    QColor color;
    color.setRgb(251,249,239);
    pe.setColor(QPalette::Window, color);
    setPalette(pe);

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

    // 设置确认密码
    pixmap = new QPixmap(":/img/resources/checkpassword.jpg");
    pixmap->scaled(ui->label_3->size(),Qt::KeepAspectRatio);
    ui->label_3->setScaledContents(true);
    ui->label_3->setPixmap(*pixmap);
    delete pixmap;
    pixmap = nullptr;

    // 设置提示不可见
    ui->error_msg->setVisible(false);
}

Register::~Register()
{
    delete ui;
}

void Register::on_cancel_button_clicked()
{
    this->close();
}

void Register::on_register_button_clicked()
{
    auto sql_opt = SqlOperator::getInstance();
    if (!sql_opt->connect()) {
        qDebug() << "failed to connect";
    }

    // 如果表不存在则创建表
    QSqlQuery* sql_result;
    if (!sql_opt->isTableExist("Users")) {
        sql_result = sql_opt->operate("CREATE TABLE Users("
                                      "username TEXT PRIMARY KEY NOT NULL, "
                                      "password TEXT NOT NULL)");
        if (nullptr == sql_result) {
            qDebug() << "Error, create table failed!";
        } else {
            sql_result->finish();
        }
    }

    // 获取用户名,密码,确认密码
    QString username_contain = this->ui->username->text();
    QString password_contain = this->ui->password->text();
    QString checkpassword_contain = this->ui->checkpassword->text();

    // 如果用户名为空,提示输入用户名
    if (username_contain.isEmpty()) {
        this->ui->error_msg->setText("请输入用户名");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        this->ui->error_msg->setPalette(pe);
        this->ui->error_msg->setVisible(true);
        sql_opt->close();
        return;
    }

    // 在表中查询username是否已经存在
    sql_result = sql_opt->operate(QString("SELECT COUNT(*) FROM `Users` WHERE `username` = \'") + username_contain + QString("\';"));
    if (nullptr == sql_result) {
        qDebug() << "Error, query failed!";
    }
    sql_result->next();
    if (sql_result->value(0).toInt() != 0) {
        this->ui->error_msg->setText("用户名已存在");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        this->ui->error_msg->setPalette(pe);
        this->ui->error_msg->setVisible(true);
        sql_result->finish();
        sql_opt->close();
        return;
    }
    sql_result->finish();

    // 如果密码为空,提示输入密码
    if (password_contain.isEmpty()) {
        this->ui->error_msg->setText("请输入密码");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        this->ui->error_msg->setPalette(pe);
        this->ui->error_msg->setVisible(true);
        sql_opt->close();
        return;
    }

    // 如果密码格式错误,提示重新输入密码
    if (!checkPassword(password_contain)) {
        this->ui->error_msg->setText("密码为6-12位,仅允许数字字母及下划线");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        this->ui->error_msg->setPalette(pe);
        this->ui->error_msg->setVisible(true);
        sql_opt->close();
        return;
    }

    // 如果确认密码为空,提示输入密码
    if (checkpassword_contain.isEmpty()) {
        this->ui->error_msg->setText("请确认密码");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        this->ui->error_msg->setPalette(pe);
        this->ui->error_msg->setVisible(true);
        sql_opt->close();
        return;
    }

    // 如果两次密码不一致,提示两次密码需要一致
    if (0 != QString::compare(checkpassword_contain, password_contain)) {
        this->ui->error_msg->setText("两次输入的密码需要保持一致");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        this->ui->error_msg->setPalette(pe);
        this->ui->error_msg->setVisible(true);
        sql_opt->close();
        return;
    }

    // 注册成功
    sql_result = SqlOperator::getInstance()->operate("INSERT INTO `Users` VALUES (\'" +
                                                     username_contain + QString("\',\'") + password_contain + QString("\');"));
    if (nullptr == sql_result) {
        qDebug() << "register failed!";
        sql_result->finish();
        sql_opt->close();
        return;
    }
    this->ui->error_msg->setText("注册成功");
    this->ui->password->setText("");
    this->ui->checkpassword->setText("");
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    this->ui->error_msg->setPalette(pe);
    this->ui->error_msg->setVisible(true);
    sql_result->finish();
    sql_opt->close();
}
