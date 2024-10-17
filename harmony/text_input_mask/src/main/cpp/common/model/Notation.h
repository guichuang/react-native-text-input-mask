#pragma once
#include <string>

namespace TinpMask {

class Notation {
public:
    // 构造函数
    Notation(const char character, const std::string &characterSet, bool isOptional)
        : character(character), characterSet(characterSet), isOptional(isOptional) {}

    // 成员变量
    char character;           // 单个字符作为字符串
    std::string characterSet; // 字符集
    bool isOptional;          // 是否可选

    // 其他方法和成员可以根据需要添加
};
} // namespace TinpMask