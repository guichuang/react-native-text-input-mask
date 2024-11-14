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
#include <vector>
#include <string>
#include <memory>
#include "model/common.h"
#include "model/Notation.h"
#include "model/State.h"
#include "FormatError.h"
#include "FormatSanitizer.h"
#include <glog/logging.h>

namespace TinpMask {
class FormatSanitizer;
class Compiler {
private:
    std::vector<Notation> customNotations;

public:
    Compiler(const std::vector<Notation> &notations) : customNotations(notations) {}

    std::shared_ptr<State> compile(const std::string &formatString) {
        FormatSanitizer sanitizer;
        std::string sanitizedString = sanitizer.sanitize(formatString);
        return compile(sanitizedString, false, false, '\0');
    }

    std::shared_ptr<State> compile(const std::string &formatString, bool valuable, bool fixed, char lastCharacter) {
        if (formatString.empty()) {
            return std::make_shared<EOLState>();
        }

        char ch = formatString.front();
        switch (ch) {
        case '[':
            if (lastCharacter != '\\') {
                return compile(formatString.substr(1), true, false, ch);
            }
            break;

        case '{':
            if (lastCharacter != '\\') {
                return compile(formatString.substr(1), false, true, ch);
            }
            break;

        case ']':
        case '}':
            if (lastCharacter != '\\') {
                return compile(formatString.substr(1), false, false, ch);
            }
            break;

        case '\\':
            if (lastCharacter != '\\') {
                return compile(formatString.substr(1), valuable, fixed, ch);
            }
            break;

        default:
            break;
        }

        if (valuable) {
            switch (ch) {
            case '0':
                return std::make_shared<ValueState>(
                    this->compile(formatString.substr(1), true, false, ch),
                    // std::static_pointer_cast<ValueState::StateType>( std::make_shared<ValueState::Numeric>())
                    std::make_shared<ValueState::Numeric>());

            case 'A':
                return std::make_shared<ValueState>(this->compile(formatString.substr(1), true, false, ch),
                                                    std::make_shared<ValueState::Literal>());

            case '_':
                return std::make_shared<ValueState>(this->compile(formatString.substr(1), true, false, ch),
                                                    std::make_shared<ValueState::AlphaNumeric>());

            case '...':
                return std::make_unique<ValueState>(determineInheritedType(lastCharacter));

            case '9':
                return std::make_shared<OptionalValueState>(this->compile(formatString.substr(1), true, false, ch),
                                                            std::make_shared<OptionalValueState::Numeric>()

                );

            case 'a':
                return std::make_shared<OptionalValueState>(this->compile(formatString.substr(1), true, false, ch),
                                                            std::make_shared<OptionalValueState::Literal>());

            case '-':
                return std::make_shared<OptionalValueState>(this->compile(formatString.substr(1), true, false, ch),
                                                            std::make_shared<OptionalValueState::AlphaNumeric>());

            default:
                return compileWithCustomNotations(ch, formatString);
            }
        }
        if (fixed) {
            return std::make_shared<FixedState>(compile(formatString.substr(1), false, true, ch), ch);
        }
        return std::make_shared<FreeState>(compile(formatString.substr(1), false, false, ch), ch);
    }

    std::unique_ptr<State> compileWithCustomNotations(char c, const std::string &str) {
        for (const auto &customNotation : customNotations) {
            if (customNotation.character == c) {
                std::shared_ptr<State> compiledState = compile(str.substr(1), true, false, c);
                if (customNotation.isOptional) {
                    return std::make_unique<OptionalValueState>(
                        std::move(compiledState),
                        std::make_shared<OptionalValueState::Custom>(c, customNotation.characterSet));
                } else {
                    return std::make_unique<ValueState>(
                        std::move(compiledState), std::make_shared<ValueState::Custom>(c, customNotation.characterSet));
                }
            }
        }
        throw FormatError("compileWithCustomNotations No Match Found");
    }
    std::shared_ptr<ValueState::ValueStateType> determineInheritedType(std::optional<char> lastCharacter) {
        if (!lastCharacter.has_value()) {
            throw FormatError(); // 处理空字符情况，抛出异常
        }
        char character = lastCharacter.value();
        switch (character) {
        case '0':
        case '9':
            return std::make_shared<ValueState::Numeric>();

        case 'A':
        case 'a':
            return std::make_shared<ValueState::Literal>();

        case '_':
        case '-':
        case '...':
        case '[':
            return std::make_shared<ValueState::AlphaNumeric>();

        default:
            return determineTypeWithCustomNotations(lastCharacter);
        }
    }

    std::shared_ptr<ValueState::ValueStateType> determineTypeWithCustomNotations(std::optional<char> lastCharacter) {
        if (!lastCharacter.has_value()) {
            throw FormatError(); // 处理空字符情况，抛出异常
        }
        char character = lastCharacter.value();
        for (const auto &customNotation : customNotations) {
            if (customNotation.character == character) {
                // 返回 Custom 状态
                return std::make_shared<ValueState::Custom>(lastCharacter.value(),
                                                            customNotation.characterSet); // 可以根据需要调整返回内容
            }
        }
        throw FormatError("determineTypeWithCustomNotations No Match Found"); // 未找到匹配项，抛出异常
    }
};
} // namespace TinpMask