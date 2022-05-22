#include "general_algorithm.h"

bool checkPassword(const QString& password) {
    // 密码规则,应当在6位-12位,且仅包含数字字母下划线
    auto len = password.length();
    if (len < 6 || len > 12) {
        return false;
    }
    // 正则表达式匹配数字字母下划线
    bool flag = password.contains(QRegExp("^[A-Za-z0-9_]+$"));
    if (true == flag) {
        return true;
    } else {
        return false;
    }
}
