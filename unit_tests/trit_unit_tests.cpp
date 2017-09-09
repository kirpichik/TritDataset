//
//  trit-unittest.cpp
//  TritDataset
//
//  Created by Кирилл on 04.09.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#include <cmath>
#include <string>

#include "gtest/gtest.h"
#include "TritSet.h"

/**
 * Преобразует строчную запись тритового числа в набор тритов.
 * @param str Строчная запись тритового числа.
 * @return Контейнер тритов.
 */
TritSet* tritSetFromString(std::string str) {
    if (!str.length())
        return new TritSet();
    
    TritSet* set = new TritSet(str.length());
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    
    size_t pos = 0;
    for (char c : str) {
        switch (c) {
            case 'f':
                set->setTrit(pos, False);
                break;
            case 'u':
                set->setTrit(pos, Unknown);
                break;
            case 't':
                set->setTrit(pos, True);
                break;
            default:
                delete set;
                return new TritSet();
        }
        pos++;
    }
    
    return set;
}

TEST(ConstructorTritSetTest, ZeroMemory) {
    TritSet set;
    
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
}

TEST(ConstructorTritSetTest, AllocatingMemory) {
    TritSet set(1000);
    
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 1000);
}

TEST(ConstructorTritSetTest, SettingDefaultValue) {
    TritSet set(1000, True);
    
    ASSERT_EQ(set.size(), 1000);
    ASSERT_GE(set.capacity(), 1000);
    
    for (size_t i = 0; i < 1000; i++)
        ASSERT_EQ(set[i], True);
}

TEST(MethodsTritSetTest, TrimSet) {
    TritSet set(100, True);
    
    ASSERT_EQ(set.size(), 100);
    ASSERT_GE(set.capacity(), 100);
    
    set.trim(50);
    
    ASSERT_EQ(set.size(), 50);
    ASSERT_GE(set.capacity(), 50);
    
    // Проверка оставшихся
    for (size_t i = 0; i < 50; i++)
        ASSERT_EQ(set.getTrit(i), True);
    
    // Проверка удаленных
    for (size_t i = 50; i < 100; i++)
        ASSERT_EQ(set.getTrit(i), Unknown);
}

TEST(MethodsTritSetTest, ShrinkSet) {
    TritSet set(1000);
    
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 1000);
    
    set.shrink();
    
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    
    set.setTrit(99, True);
    
    ASSERT_EQ(set.size(), 100);
    ASSERT_GE(set.capacity(), 100);
    
    set.setTrit(49, False);
    
    ASSERT_EQ(set.size(), 100);
    ASSERT_GE(set.capacity(), 100);
    
    set.setTrit(99, Unknown);
    
    ASSERT_EQ(set.size(), 50);
    ASSERT_GE(set.capacity(), 100);
    
    set.shrink();
    
    ASSERT_EQ(set.size(), 50);
    ASSERT_GE(set.capacity(), 50);
}

TEST(MethodsTritSetTest, GetTrit) {
    TritSet set;
    
    ASSERT_EQ(set.setTrit(0, True).getTrit(0), True);
    ASSERT_EQ(set.setTrit(10, True).getTrit(10), True);
}

TEST(MethodsTritSetTest, GetSetTrit) {
    TritSet set;
    
    ASSERT_EQ(set.setTrit(0, Unknown).size(), 0);
    ASSERT_EQ(set.capacity(), 0);
    
    ASSERT_EQ(set.setTrit(10, Unknown).size(), 0);
    ASSERT_EQ(set.capacity(), 0);
    
    set.setTrit(0, True);
    ASSERT_EQ(set.size(), 1);
    ASSERT_GE(set.capacity(), 1);
    
    set.setTrit(9, True);
    
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
}

TEST(MethodsTritSetTest, Cardinality) {
    TritSet emptySet, setFalse(100, False), setUnknown(100, Unknown), setTrue(100, True);
    
    std::unordered_map<Trit, size_t, std::hash<size_t>> map = emptySet.cardinality();
    
    ASSERT_EQ(map.at(False), 0);
    ASSERT_EQ(map.at(Unknown), 0);
    ASSERT_EQ(map.at(True), 0);
    
    map = setFalse.cardinality();
    
    ASSERT_EQ(map.at(False), 100);
    ASSERT_EQ(map.at(Unknown), 0);
    ASSERT_EQ(map.at(True), 0);
    
    map = setUnknown.cardinality();
    
    ASSERT_EQ(map.at(False), 0);
    ASSERT_EQ(map.at(Unknown), 0);
    ASSERT_EQ(map.at(True), 0);
    
    map = setTrue.cardinality();
    
    ASSERT_EQ(map.at(False), 0);
    ASSERT_EQ(map.at(Unknown), 0);
    ASSERT_EQ(map.at(True), 100);
    
    TritSet* set = tritSetFromString("FFFUUUTTT");
    
    map = set->cardinality();
    
    ASSERT_EQ(map.at(False), 3);
    ASSERT_EQ(map.at(Unknown), 3);
    ASSERT_EQ(map.at(True), 3);
}

/** NOT оператор. */

TEST(OperatorsTritSetTest, OperatorNOTEmpty) {
    TritSet emptySet;
    
    ASSERT_EQ(emptySet.size(), 0);
    ASSERT_GE(emptySet.capacity(), 0);
    
    emptySet = ~emptySet;
    
    ASSERT_EQ(emptySet.size(), 0);
    ASSERT_GE(emptySet.capacity(), 0);
}

TEST(OperatorsTritSetTest, OperatorNOTFalse) {
    TritSet setFalse(10, False);
    
    ASSERT_EQ(setFalse.size(), 10);
    ASSERT_GE(setFalse.capacity(), 10);
    
    setFalse = ~setFalse;
    
    ASSERT_EQ(setFalse.size(), 10);
    ASSERT_GE(setFalse.capacity(), 10);
    
    for (size_t i = 0; i < 10; i++)
        ASSERT_EQ(setFalse.getTrit(i), True);
}

TEST(OperatorsTritSetTest, OperatorNOTUnknown) {
    TritSet setUnknown(10, Unknown);
    
    ASSERT_EQ(setUnknown.size(), 0);
    ASSERT_GE(setUnknown.capacity(), 10);
    
    setUnknown = ~setUnknown;
    
    ASSERT_EQ(setUnknown.size(), 0);
    ASSERT_GE(setUnknown.capacity(), 0);
    
    for (size_t i = 0; i < 10; i++)
        ASSERT_EQ(setUnknown.getTrit(i), Unknown);
}

TEST(OperatorsTritSetTest, OperatorNOTTrue) {
    TritSet setTrue(10, True);
    
    ASSERT_EQ(setTrue.size(), 10);
    ASSERT_GE(setTrue.capacity(), 10);
    
    setTrue = ~setTrue;
    
    ASSERT_EQ(setTrue.size(), 10);
    ASSERT_GE(setTrue.capacity(), 10);
    
    for (size_t i = 0; i < 10; i++)
        ASSERT_EQ(setTrue.getTrit(i), False);
}

TEST(OperatorsTritSetTest, OperatorNOTAll) {
    TritSet* setAll = tritSetFromString("FUTUTF");
    
    ASSERT_EQ(setAll->size(), 6);
    ASSERT_GE(setAll->capacity(), 6);
    
    *setAll = ~(*setAll);
    
    TritSet* notSet = tritSetFromString("TUFUFT");
    
    ASSERT_EQ((*setAll), (*notSet));
    
    delete setAll;
    delete notSet;
}

/** OR оператор. */

TEST(OperatorsTritSetTest, OperatorORContinually) {
    TritSet setEmpty;
    TritSet setTrue(10, True), setFalse(10, False), setUnknown(10, Unknown);

    ASSERT_EQ(setEmpty.size(), 0);
    ASSERT_GE(setEmpty.capacity(), 0);
    
    ASSERT_EQ(setTrue.size(), 10);
    ASSERT_GE(setTrue.capacity(), 10);
    
    ASSERT_EQ(setFalse.size(), 10);
    ASSERT_GE(setFalse.capacity(), 10);
    
    ASSERT_EQ(setUnknown.size(), 0);
    ASSERT_GE(setUnknown.capacity(), 10);
    
    // Empty | Empty
    TritSet set = setEmpty | setEmpty;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    ASSERT_EQ(set, setEmpty);
    
    // True | Empty
    set = setTrue | setEmpty;
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set, setTrue);
    
    // Unknown | Empty
    set = setUnknown | setEmpty;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    ASSERT_EQ(set, setEmpty);
    
    // False | Empty
    set = setFalse | setEmpty;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    ASSERT_EQ(set, setEmpty);
    
    // False | False
    set = setFalse | setFalse;
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set, setFalse);
    
    // True | True
    set = setTrue | setTrue;
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set, setTrue);
    
    // True | False
    set = setTrue | setFalse;
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set, setTrue);
    
    // Unknown | Unknown
    set = setUnknown | setUnknown;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    ASSERT_EQ(set, setEmpty);
    
    // Unknown | True
    set = setUnknown | setTrue;
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set, setTrue);
    
    // Unknown | False
    set = setUnknown | setFalse;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    ASSERT_EQ(set, setEmpty);
}

TEST(OperatorsTritSetTest, OperatorORDifferent) {
    TritSet* setLeft = tritSetFromString("FUT");
    TritSet* setRight = tritSetFromString("TFU");
    TritSet* setLeftSmall = tritSetFromString("TU");
    TritSet* setRightSmall = tritSetFromString("UF");
    
    TritSet set = (*setLeft) | (*setRight);
    ASSERT_EQ(set.size(), 3);
    ASSERT_GE(set.capacity(), 3);
    TritSet* temp = tritSetFromString("TUT");
    ASSERT_EQ(set, (*temp));
    delete temp;
    
    set = (*setLeft) | (*setLeftSmall);
    ASSERT_EQ(set.size(), 3);
    ASSERT_GE(set.capacity(), 3);
    temp = tritSetFromString("TUT");
    ASSERT_EQ(set, (*temp));
    delete temp;
    
    set = (*setRight) | (*setRightSmall);
    ASSERT_EQ(set.size(), 2);
    ASSERT_GE(set.capacity(), 2);
    temp = tritSetFromString("TF");
    ASSERT_EQ(set, (*temp));
    delete temp;
    
    set = (*setRight) | (*setLeftSmall);
    ASSERT_EQ(set.size(), 1);
    ASSERT_GE(set.capacity(), 1);
    temp = tritSetFromString("T");
    ASSERT_EQ(set, (*temp));
    delete temp;
    
    delete setLeft;
    delete setRight;
    delete setLeftSmall;
    delete setRightSmall;
}

/** AND оператор. */

TEST(OperatorsTritSetTest, OperatorANDContinually) {
    TritSet setEmpty;
    TritSet setTrue(10, True), setFalse(10, False), setUnknown(10, Unknown);
    
    ASSERT_EQ(setEmpty.size(), 0);
    ASSERT_GE(setEmpty.capacity(), 0);
    
    ASSERT_EQ(setTrue.size(), 10);
    ASSERT_GE(setTrue.capacity(), 10);
    
    ASSERT_EQ(setFalse.size(), 10);
    ASSERT_GE(setFalse.capacity(), 10);
    
    ASSERT_EQ(setUnknown.size(), 0);
    ASSERT_GE(setUnknown.capacity(), 10);
    
    // Empty & Empty
    TritSet set = setEmpty & setEmpty;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    ASSERT_EQ(set, setEmpty);
    
    // True & Empty
    set = setTrue & setEmpty;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    ASSERT_EQ(set, setEmpty);
    
    // Unknown & Empty
    set = setUnknown & setEmpty;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    ASSERT_EQ(set, setEmpty);
    
    // False & Empty
    set = setFalse & setEmpty;
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set, setFalse);
    
    // False & False
    set = setFalse & setFalse;
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set, setFalse);
    
    // True & True
    set = setTrue & setTrue;
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set, setTrue);
    
    // True & False
    set = setTrue & setFalse;
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set, setFalse);
    
    // Unknown & Unknown
    set = setUnknown & setUnknown;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    ASSERT_EQ(set, setEmpty);
    
    // Unknown & True
    set = setUnknown & setTrue;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    ASSERT_EQ(set, setEmpty);
    
    // Unknown & False
    set = setUnknown & setFalse;
    ASSERT_EQ(set.size(), 10);
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set, setFalse);
}

TEST(OperatorsTritSetTest, OperatorANDDifferent) {
    TritSet* setLeft = tritSetFromString("FUT");
    TritSet* setRight = tritSetFromString("TFU");
    TritSet* setLeftSmall = tritSetFromString("TU");
    TritSet* setRightSmall = tritSetFromString("UF");
    
    TritSet set = (*setLeft) & (*setRight);
    ASSERT_EQ(set.size(), 2);
    ASSERT_GE(set.capacity(), 2);
    TritSet* temp = tritSetFromString("FF");
    ASSERT_EQ(set, (*temp));
    delete temp;
    
    set = (*setLeft) & (*setLeftSmall);
    ASSERT_EQ(set.size(), 1);
    ASSERT_GE(set.capacity(), 1);
    temp = tritSetFromString("F");
    ASSERT_EQ(set, (*temp));
    delete temp;
    
    set = (*setRight) & (*setRightSmall);
    ASSERT_EQ(set.size(), 2);
    ASSERT_GE(set.capacity(), 2);
    temp = tritSetFromString("UF");
    ASSERT_EQ(set, (*temp));
    delete temp;
    
    set = (*setRight) & (*setLeftSmall);
    ASSERT_EQ(set.size(), 2);
    ASSERT_GE(set.capacity(), 2);
    temp = tritSetFromString("TF");
    ASSERT_EQ(set, (*temp));
    delete temp;
    
    delete setLeft;
    delete setRight;
    delete setLeftSmall;
    delete setRightSmall;
}

/** Оператор сравнения. */

TEST(OperatorsTritSetTest, OperatorEquals) {
    TritSet setTrue(10, True), setFalse(10, False), setUnknown(10, Unknown);
    TritSet* setLeft = tritSetFromString("FUT");
    TritSet* setRight = tritSetFromString("UTF");
    TritSet* setSmall = tritSetFromString("TF");
    
    ASSERT_EQ(setTrue, setTrue);
    ASSERT_EQ(setFalse, setFalse);
    ASSERT_EQ(setUnknown, setUnknown);
    ASSERT_EQ((*setLeft), (*setLeft));
    ASSERT_EQ((*setSmall), (*setSmall));
    
    ASSERT_NE((*setLeft), (*setRight));
    ASSERT_NE(setTrue, setFalse);
    ASSERT_NE(setTrue, setUnknown);
    ASSERT_NE(setFalse, setUnknown);
    
    delete setLeft;
    delete setRight;
    delete setSmall;
}

/** Оператор получения по индексу. */
TEST(OperatorsTritSetTest, OperatorGet) {
    TritSet set;
    
    ASSERT_EQ(set.setTrit(0, True)[0], True);
    ASSERT_EQ(set.setTrit(10, True)[10], True);
}

/** Оператор установки по индексу. */
TEST(OperatorsTritSetTest, OperatorSet) {
    TritSet set;
    
    set[0] = Unknown;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    
    set[10] = Unknown;
    ASSERT_EQ(set.size(), 0);
    ASSERT_GE(set.capacity(), 0);
    
    set[0] = True;
    ASSERT_GE(set.capacity(), 1);
    ASSERT_EQ(set.size(), 1);
    
    set[9] = True;
    ASSERT_GE(set.capacity(), 10);
    ASSERT_EQ(set.size(), 10);
}
