#include "keyhelp.h"
#include "ui_keyhelp.h"

KeyHelp::KeyHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyHelp)
{
    ui->setupUi(this);

    // 背景颜色
    QPalette pe = palette();
    QColor color;
    color.setRgb(251,249,239);
    pe.setColor(QPalette::Window, color);
    setPalette(pe);

    // 设置自动换行
    ui->label->setWordWrap(true);
}

KeyHelp::~KeyHelp()
{
    delete ui;
}
