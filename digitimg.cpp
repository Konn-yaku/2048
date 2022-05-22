#include "digitimg.h"

DigitImg::DigitImg() : QLabel(), index(0), val(0) {}

void DigitImg::setIndex(char index) {
    this->index = index;
}

char DigitImg::getIndex() const {
    return index;
}

void DigitImg::setValue(char val) {
    this->val = val;
}

char DigitImg::getValue() const {
    return this->val;
}
