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
#include <stdexcept>
#include <limits>
#include "../Mask.h"     // 确保包含 Mask 头文件
#include "CaretString.h" // 确保包含 CaretString 头文件

namespace TinpMask {

enum class AffinityCalculationStrategy { WHOLE_STRING, PREFIX, CAPACITY, EXTRACTED_VALUE_CAPACITY };

class AffinityCalculator {
public:
    static int calculateAffinityOfMask(AffinityCalculationStrategy strategy,  Mask &mask,
                                       const CaretString &text) {
        switch (strategy) {
        case AffinityCalculationStrategy::WHOLE_STRING:
            return mask.apply(text).affinity;

        case AffinityCalculationStrategy::PREFIX:
            return prefixIntersection(mask.apply(text).formattedText.string, text.string).length();

        case AffinityCalculationStrategy::CAPACITY:
            return text.string.length() > mask.totalTextLength() ? std::numeric_limits<int>::min()
                                                                 : text.string.length() - mask.totalTextLength();

        case AffinityCalculationStrategy::EXTRACTED_VALUE_CAPACITY: {
            const auto &extractedValue = mask.apply(text).extractedValue;
            return extractedValue.length() > mask.totalValueLength()
                       ? std::numeric_limits<int>::min()
                       : extractedValue.length() - mask.totalValueLength();
        }
        default:
            throw std::invalid_argument("Unknown AffinityCalculationStrategy");
        }
    }

private:
    // Helper function to find prefix intersection
    static std::string prefixIntersection(const std::string &str1, const std::string &str2) {
        size_t endIndex = 0;
        while (endIndex < str1.length() && endIndex < str2.length()) {
            if (str1[endIndex] == str2[endIndex]) {
                endIndex += 1;
            } else {
                return str1.substr(0, endIndex);
            }
        }
        return str1.substr(0, endIndex);
    }
};
} // namespace TinpMask