/**
 * MIT License
 *
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once
#include <string>
#include <memory>

namespace TinpMask {

class CaretString {
    // 光标重力基类
public:
    class CaretGravity {
    public:
        virtual ~CaretGravity() = default; // 虚析构函数

        // 获取自动完成值
        virtual bool autocomplete() const { return false; }
        // 获取自动跳过值
        virtual bool autoskip() const { return false; }
    };

    // 向前的光标重力
    class Forward : public CaretGravity {
    public:
        Forward(bool autoCompleteValue) : autocompleteValue(autoCompleteValue) {}

        bool autocomplete() const override { return autocompleteValue; }

    private:
        bool autocompleteValue; // 自动完成值
    };

    // 向后的光标重力
    class Backward : public CaretGravity {
    public:
        Backward(bool autoSkipValue) : autoskipValue(autoSkipValue) {}

        bool autoskip() const override { return autoskipValue; }

    private:
        bool autoskipValue; // 自动跳过值
    };

public:
    // 构造函数
    CaretString(const std::string &str, int caretPos, std::shared_ptr<CaretGravity> caretGrav)
        : string(str), caretPosition(caretPos), caretGravity(caretGrav) {}

    // 反转字符串并返回新的 CaretString 对象
    CaretString reversed() const {
        // 创建一个反转后的字符串
        std::string reversedStr(string.rbegin(), string.rend());
        // 计算新的 caretPosition
        int newCaretPos = string.length() - caretPosition;
        return CaretString(reversedStr, newCaretPos, caretGravity);
    }

    // 获取字符串
    const std::string &getString() const { return string; }
    // 获取光标位置
    int getCaretPosition() const { return caretPosition; }
    // 获取光标重力
    std::shared_ptr<CaretGravity> getCaretGravity() const { return caretGravity; }


public:
    std::string string;                         // 字符串内容
    int caretPosition;                          // 光标位置
    std::shared_ptr<CaretGravity> caretGravity; // 光标重力
};

} // namespace TinpMask