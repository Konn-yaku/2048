#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <qdebug.h>
#include "sqloperator.h"
#include "general_algorithm.h"

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();


private slots:
    void on_cancel_button_clicked();

    void on_register_button_clicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
