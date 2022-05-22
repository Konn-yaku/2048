#ifndef RANKLIST_H
#define RANKLIST_H

#include <QDialog>
#include "sqloperator.h"

namespace Ui {
class RankList;
}

class RankList : public QDialog
{
    Q_OBJECT

public:
    explicit RankList(QWidget *parent = nullptr);
    ~RankList();

    void setTable();

private:
    Ui::RankList *ui;
};

#endif // RANKLIST_H
