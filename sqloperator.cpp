#include "sqloperator.h"

SqlOperator* SqlOperator::_instance = nullptr;

SqlOperator* SqlOperator::getInstance() {
    if (SqlOperator::_instance == nullptr) {
        SqlOperator::_instance = new SqlOperator();
    }
    return _instance;
}

SqlOperator::Deletor::~Deletor() {
    delete SqlOperator::_instance;
}

SqlOperator::SqlOperator() {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        this->database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        // 建立和SQlite数据库的连接
        this->database = QSqlDatabase::addDatabase("QSQLITE");
        // 设置数据库文件的名字
        this->database.setDatabaseName("DataBase.db");
        this->database.setUserName("root");
        this->database.setPassword("123456");
    }
}

SqlOperator::~SqlOperator() {}

bool SqlOperator::connect() {
    if (!this->database.open()) {
        qDebug() << this->database.lastError();
        return false;
    }
    return true;
}

void SqlOperator::close() {
    this->database.close();
}

QSqlQuery* SqlOperator::operate(const QString& sql) {
    static QSqlQuery query;
    query.prepare(sql);
    if (!query.exec()) {
        qDebug() << "Error:" << query.lastError();
        return nullptr;
    } else {
        return &query;
    }
}

bool SqlOperator::isTableExist(const QString &table_name) {
    if (this->database.tables().contains(table_name)) {
        return true;
    } else {
        return false;
    }
}
