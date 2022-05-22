#ifndef DIGITIMG_H
#define DIGITIMG_H

#include <QLabel>
#include <QRect>

class DigitImg : public QLabel
{
public:
    DigitImg();

    void setIndex(char index);
    char getIndex() const;

    void setValue(char val);
    char getValue() const;

private:
    char index;
    char val;
};

const QRect img_pos[] = {
    QRect(74, 182, 84, 79), QRect(174, 182, 84, 79), QRect(273, 182, 84, 79), QRect(373, 182, 84, 79),
    QRect(74, 274, 84, 79), QRect(174, 274, 84, 79), QRect(273, 274, 84, 79), QRect(373, 274, 84, 79),
    QRect(74, 367, 84, 79), QRect(174, 367, 84, 79), QRect(273, 367, 84, 79), QRect(373, 367, 84, 79),
    QRect(74, 460, 84, 79), QRect(174, 460, 84, 79), QRect(273, 460, 84, 79), QRect(373, 460, 84, 79),
};

#endif // DIGITIMG_H
