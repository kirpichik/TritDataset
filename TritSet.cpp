//
//  TritSet.cpp
//  TritDataset
//
//  Created by Кирилл on 06.09.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <cmath>

#include "TritSet.h"

#define FALSE_BIT_MASK 0b01
#define UNKNOWN_BIT_MASK 0b00
#define TRUE_BIT_MASK 0b10

TritSet::TritSet(size_t tritsCount, Trit defaultValue) {
    
    lastTritPos = 0;
    
    for (size_t i = 0; i < tritsCount; i++)
        _setTrit(i, defaultValue);
    
    if (defaultValue != Trit(Unknown))
        lastTritPos = tritsCount - 1;
}

size_t TritSet::capacity() const {
    return !storage.size() ? 0 : (storage.size() + 1) * sizeof(uint) * 8 / 2;
}

size_t TritSet::size() const {
    return lastTritPos || getTrit(0) != Unknown ? lastTritPos + 1 : 0;
}

Trit TritSet::getTrit(size_t pos) const {
    size_t uintPos = pos * 2 / 8 / sizeof(uint);
    
    if (uintPos > storage.size() || !storage.size())
        return Trit(Unknown);
    
    uint data = storage[uintPos];
    pos -= uintPos * sizeof(uint) * 8 / 2;
    
    if (data & (FALSE_BIT_MASK << (pos * 2)))
        return Trit(False);
    else if (data & (TRUE_BIT_MASK << (pos * 2)))
        return Trit(True);
    
    return Trit(Unknown);
}

TritSet& TritSet::trim(size_t from) {
    
    // TODO - переписать более оптимизированно
    // Сначала удалять лишние блоки uint сразу из storage,
    // а после удалять оставшиеся триты с последенего.
    // Таким образом не придется вызывать позже shrink и
    // выполнять операции с кучей лишних тритов.
    
    size_t removeCount = lastTritPos - from;
    
    for (size_t i = 0; i < removeCount; i++)
        _setTrit(i + from, Trit(Unknown));
    
    shrink();
    countLastTritPos();
    
    return *this;
}

TritSet& TritSet::shrink() {
    size_t diff = storage.size() - ceil(lastTritPos * 2 / 8. / sizeof(uint));
    
    for (size_t i = 0; i < diff; i++)
        storage.pop_back();
    
    return *this;
}

TritSet& TritSet::setTrit(size_t pos, Trit value) {
    if (getTrit(pos) == value)
        return *this;
    _setTrit(pos, value);
    countLastTritPos();
    return *this;
}

bool TritSet::operator==(const TritSet& set) const {
    if (set.size() != size())
        return false;
    
    for (size_t i = 0; i < size(); i++)
        if (getTrit(i) != set.getTrit(i))
            return false;
    
    return true;
}

bool TritSet::operator!=(const TritSet& set) const {
    return !(*this == set);
}

TritSet::ModifiableTrit TritSet::operator[](size_t pos) const {
    return TritSet::ModifiableTrit(const_cast<TritSet&>(*this), pos);
}

TritSet TritSet::operator~() const {
    TritSet result;
    for (size_t i = 0; i < size(); i++)
        result._setTrit(i, ~getTrit(i));
    result.countLastTritPos();
    return result;
}

TritSet TritSet::operator&(const TritSet& set) const {
    TritSet result;
    
    size_t maxSize = size();
    if (set.size() > size())
        maxSize = set.size();
    
    for (size_t i = 0; i < maxSize; i++)
        if (i <= lastTritPos && i <= set.lastTritPos)
            result._setTrit(i, getTrit(i) & set.getTrit(i));
        else if (i <= lastTritPos)
            result._setTrit(i, getTrit(i) & Trit(Unknown));
        else
            result._setTrit(i, set.getTrit(i) & Trit(Unknown));
    
    result.countLastTritPos();
    
    return result;
}

TritSet TritSet::operator|(const TritSet& set) const {
    TritSet result;
    
    size_t maxSize = size();
    if (set.size() > size())
        maxSize = set.size();
    
    for (size_t i = 0; i < maxSize; i++)
        if (i <= lastTritPos && i <= set.lastTritPos)
            result._setTrit(i, getTrit(i) | set.getTrit(i));
        else if (i <= lastTritPos)
            result._setTrit(i, getTrit(i) | Trit(Unknown));
        else
            result._setTrit(i, set.getTrit(i) | Trit(Unknown));
    
    result.countLastTritPos();
    
    return result;
}

std::ostream& TritSet::operator<<(std::ostream& stream) {
    for (size_t i = 0; i < size(); i++) {
        switch (getTrit(i)) {
            case False:
                stream << 'F';
                break;
            case Unknown:
                stream << 'U';
                break;
            case True:
                stream << 'T';
        }
    }
    return stream;
}

void TritSet::_setTrit(size_t pos, Trit value) {
    size_t uintPos = ceil(pos * 2 / 8 / sizeof(uint));
    pos -= uintPos * sizeof(uint) * 8 / 2;
    
    if (!storage.size())
        storage.push_back(0);
    
    if (uintPos + 1 > storage.size()) {
        size_t addingUInts = uintPos + 1 - storage.size();
        
        for (size_t i = 0; i < addingUInts; i++) // Пополняем хранилище
            storage.push_back(0);
    }
        
    uint data = storage[uintPos];
    data &= ~(11 << (pos * 2)); // Сбрасываем биты
    
    uint mask;
    switch (value) {
        case Trit(False):
            mask = FALSE_BIT_MASK;
            break;
        case Trit(Unknown):
            mask = UNKNOWN_BIT_MASK;
            break;
        case Trit(True):
            mask = TRUE_BIT_MASK;
            break;
    }
    
    storage[uintPos] = data | (mask << (pos * 2));
    
}

void TritSet::countLastTritPos() {
    size_t allowedPos = storage.size() * sizeof(uint) * 8 / 2;
    
    for (size_t i = 0; i < allowedPos; i++)
        if (getTrit(i) != Trit(Unknown))
            lastTritPos = i;
}

Trit operator~(const Trit& trit) {
    switch (trit) {
        case False:
            return True;
        case Unknown:
            return Unknown;
        case True:
            return False;
    }
}

Trit operator&(const Trit& left, const Trit& right) {
    switch (left) {
        case False:
            return False;
        case Unknown:
            return right == False ? False : Unknown;
        case True:
            return right == False ? False : (right == True ? True : Unknown);
    }
}

Trit operator|(const Trit& left, const Trit& right) {
    switch (left) {
        case False:
            return right == True ? True : (right == False ? False : Unknown);
        case Unknown:
            return right == True ? True : Unknown;
        case True:
            return True;
    }
}

