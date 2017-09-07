//
//  TritSet.cpp
//  TritDataset
//
//  Created by Кирилл on 06.09.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <cmath>

#include "TritSet.h"

TritSet::TritSet(size_t tritsCount, Trit defaultValue) {
    lastTritPos = 0;
    
    for (size_t i = 0; i < tritsCount; i++)
        _setTrit(i, defaultValue);
    
    if (defaultValue != Trit(Unknown))
        lastTritPos = tritsCount;
}

size_t TritSet::capacity() const {
    return storage.size();
}

size_t TritSet::size() const {
    return lastTritPos + 1;
}

Trit TritSet::getTrit(size_t pos) const {
    // TODO
    return Trit(Unknown);
}

TritSet& TritSet::trim(size_t from) {
    // TODO
    return *this;
}

TritSet& TritSet::shrink() {
    size_t diff = storage.size() - ceil(lastTritPos * 2 / 8. / sizeof(uint));
    
    for (size_t i = 0; i < diff; i++)
        storage.pop_back();
    
    return *this;
}

TritSet& TritSet::setTrit(size_t pos, Trit value) {
    if (value == Trit(Unknown))
        return *this;
    _setTrit(pos, value);
    countLastTritPos();
    return *this;
}

bool TritSet::operator==(const TritSet& set) const {
    if (set.lastTritPos != lastTritPos)
        return false;
    
    for (size_t i = 0; i < lastTritPos; i++)
        if (storage[i] != set.storage[i])
            return false;
    
    return true;
}

bool TritSet::operator!=(const TritSet& set) const {
    return (*this == set);
}

Trit TritSet::operator[](size_t pos) const {
    // TODO
    return Trit(Unknown);
}

TritSet TritSet::operator~() const {
    // TODO
    return TritSet();
}

TritSet TritSet::operator&(const TritSet& set) const {
    // TODO
    return TritSet();
}

TritSet TritSet::operator|(const TritSet& set) const {
    // TODO
    return TritSet();
}

std::ostream& TritSet::operator<<(std::ostream& stream) const {
    for (size_t i = 0; i < lastTritPos; i++) {
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
    // TODO
}

void TritSet::countLastTritPos() {
    size_t allowedPos = storage.size() * sizeof(uint) * 8 / 2;
    
    for (size_t i = 0; i < allowedPos; i++)
        if (getTrit(i) != Trit(Unknown))
            lastTritPos = i;
}
