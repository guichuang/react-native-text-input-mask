#pragma once
#include "CaretString.h"
#include "CaretStringIterator.h"

namespace TinpMask {

class RTLCaretStringIterator : public CaretStringIterator {
public:
    RTLCaretStringIterator(const CaretString& caretString)
        : CaretStringIterator(caretString) {}

    bool insertionAffectsCaret()  {
        return currentIndex <= caretString.caretPosition;
    }
};

}