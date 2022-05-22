#ifndef KEYHELP_H
#define KEYHELP_H

#include <QDialog>

namespace Ui {
class KeyHelp;
}

class KeyHelp : public QDialog
{
    Q_OBJECT

public:
    explicit KeyHelp(QWidget *parent = nullptr);
    ~KeyHelp();

private:
    Ui::KeyHelp *ui;
};

#endif // KEYHELP_H
