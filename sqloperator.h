// 采用单例设计模式
#ifndef SQLOPERATOR_H
#define SQLOPERATOR_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <qdebug.h>

class SqlOperator
{
public:
    static SqlOperator* getInstance();

public:
    bool connect();
    void close();
    bool isTableExist(const QString& table_name);

    QSqlQuery* operate(const QString& sql);

//private:
    static SqlOperator* _instance;

    // 防止内存泄漏
    class Deletor {
    public:
        ~Deletor();
    };
    static Deletor deletor;

    SqlOperator();
    ~SqlOperator();

    QSqlDatabase database;
};

#endif // SQLOPERATOR_H
