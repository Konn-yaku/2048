#include "ranklist.h"
#include "ui_ranklist.h"

RankList::RankList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RankList)
{
    ui->setupUi(this);
}

void RankList::setTable() {
    // 设置背景
    QPalette pe = palette();
    QColor color;
    color.setRgb(251,249,239);
    pe.setColor(QPalette::Window, color);
    setPalette(pe);

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

    // 设置表格内容
    sql_result = sql_opt->operate("SELECT * FROM `Scores` ORDER BY `score` DESC;");

    if (nullptr == sql_result) {
        qDebug() << "Failed to query";
    }

    // 设置表格
    ui->table->setRowCount(10);
    ui->table->setColumnCount(2);
    ui->table->setHorizontalHeaderLabels(QStringList() << "用户名" << "得分");
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 表格不允许编辑

    int index = 0;
    while (index < 10 && sql_result->next()) {
        ui->table->setItem(index, 0, new QTableWidgetItem(sql_result->value(1).toString()));
        ui->table->setItem(index++, 1, new QTableWidgetItem(sql_result->value(2).toString()));
    }
}

RankList::~RankList()
{
    delete ui;
}
