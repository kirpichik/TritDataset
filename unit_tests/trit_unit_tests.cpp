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
TritSet tritSetFromString(std::string str) {
    if (!str.length())
        return TritSet();
    
    TritSet set(str.length());
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    
    size_t pos = 0;
    for (char c : str) {
        switch (c) {
            case 'F':
                set.setTrit(pos, Trit(False));
                break;
            case 'U':
                set.setTrit(pos, Trit(Unknown));
                break;
            case 'T':
                set.setTrit(pos, Trit(True));
                break;
            default:
                return TritSet();
        }
        pos++;
    }
    
    return set;
}

/**
 * Вычисляет размер требуемой памяти в байтах для данного кол-ва тритов.
 * @param tritsCount Кол-во тритов для хранения.
 * @return Кол-во требуемой памяти для хранения tritsCount тритов.
 */
size_t getCapacity(size_t tritsCount) {
    return ceil(tritsCount * 2 / 8. / sizeof(uint));
}

/**
 * Выполняет проверку на логический размер и размер затраченой памяти.
 * @param set Контейнер тритов.
 * @param size Логический размер.
 * @param capacity Размер затраченной памяти.
 */
void assertSizeAndCapacity(TritSet& set, size_t size, size_t capacity) {
    ASSERT_EQ(set.capacity(), size);
    ASSERT_EQ(set.size(), capacity);
}

/**
 * Выполняет проверку на логический размер и размер затраченой памяти при условии,
 * что они эквиваленты.
 * @param set - Контейнер тритов.
 * @param size - Логический размер.
 */
void assertEqualSizeAndCapacity(TritSet& set, size_t size) {
    assertSizeAndCapacity(set, size, size);
}

TEST(ConstructorTritSetTest, ZeroMemory) {
    TritSet set;
    
    assertEqualSizeAndCapacity(set, 0);
}

TEST(ConstructorTritSetTest, AllocatingMemory) {
    TritSet set(1000);
    
    assertSizeAndCapacity(set, 0, 1000);
}

TEST(ConstructorTritSetTest, SettingDefaultValue) {
    TritSet set(1000, Trit(True));
    
    assertEqualSizeAndCapacity(set, 1000);
    
    for (size_t i = 0; i < 1000; i++)
        ASSERT_EQ(set[i], Trit(True));
}

TEST(MethodsTritSetTest, TrimSet) {
    TritSet set(100, Trit(True));
    
    assertEqualSizeAndCapacity(set, 100);
    
    set.trim(50);
    
    assertEqualSizeAndCapacity(set, 50);
    
    // Проверка оставшихся
    for (size_t i = 0; i < 50; i++)
        ASSERT_EQ(set.getTrit(i), Trit(True));
    
    // Проверка удаленных
    for (size_t i = 50; i < 100; i++)
        ASSERT_EQ(set.getTrit(i), Trit(Unknown));
}

TEST(MethodsTritSetTest, ShrinkSet) {
    TritSet set(1000);
    
    assertSizeAndCapacity(set, 0, 1000);
    
    set.shrink();
    
    assertEqualSizeAndCapacity(set, 0);
    
    set.setTrit(100, Trit(True));
    
    assertEqualSizeAndCapacity(set, 100);
    
    set.setTrit(50, Trit(False));
    
    assertEqualSizeAndCapacity(set, 100);
    
    set.setTrit(100, Trit(Unknown));
    
    assertSizeAndCapacity(set, 50, 100);
    
    set.shrink();
    
    assertEqualSizeAndCapacity(set, 50);
}

TEST(MethodsTritSetTest, GetTrit) {
    TritSet set;
    
    ASSERT_EQ(set.setTrit(0, Trit(True)).getTrit(0), Trit(True));
    ASSERT_EQ(set.setTrit(10, Trit(True)).getTrit(10), Trit(True));
}

TEST(MethodsTritSetTest, GetSetTrit) {
    TritSet set;
    
    ASSERT_EQ(set.setTrit(0, Trit(Unknown)).size(), 0);
    ASSERT_EQ(set.capacity(), 0);
    
    ASSERT_EQ(set.setTrit(10, Trit(Unknown)).size(), 0);
    ASSERT_EQ(set.capacity(), 0);
    
    set.setTrit(0, Trit(True));
    assertEqualSizeAndCapacity(set, 1);
    
    set.setTrit(10, Trit(True));
    assertEqualSizeAndCapacity(set, 10);
}

/** NOT оператор. */

TEST(OperatorsTritSetTest, OperatorNOTEmpty) {
    TritSet emptySet;
    
    assertEqualSizeAndCapacity(emptySet, 0);
    
    emptySet = ~emptySet;
    
    assertEqualSizeAndCapacity(emptySet, 0);
}

TEST(OperatorsTritSetTest, OperatorNOTFalse) {
    TritSet setFalse(10, Trit(False));
    
    assertEqualSizeAndCapacity(setFalse, 10);
    
    setFalse = ~setFalse;
    
    assertEqualSizeAndCapacity(setFalse, 10);
    for (size_t i = 0; i < 10; i++)
        ASSERT_EQ(setFalse.getTrit(i), Trit(True));
}

TEST(OperatorsTritSetTest, OperatorNOTUnknown) {
    TritSet setUnknown(10, Trit(Unknown));
    
    assertEqualSizeAndCapacity(setUnknown, 10);
    
    setUnknown = ~setUnknown;
    
    assertEqualSizeAndCapacity(setUnknown, 10);
    for (size_t i = 0; i < 10; i++)
        ASSERT_EQ(setUnknown.getTrit(i), Trit(Unknown));
}

TEST(OperatorsTritSetTest, OperatorNOTTrue) {
    TritSet setTrue(10, Trit(True));
    
    assertEqualSizeAndCapacity(setTrue, 10);
    
    setTrue = ~setTrue;
    
    assertEqualSizeAndCapacity(setTrue, 10);
    for (size_t i = 0; i < 10; i++)
        ASSERT_EQ(setTrue.getTrit(i), Trit(False));
}

TEST(OperatorsTritSetTest, OperatorNOTAll) {
    TritSet setAll = tritSetFromString("FUTUTF");
    
    assertEqualSizeAndCapacity(setAll, 6);
    
    ASSERT_EQ(setAll, tritSetFromString("TUFUFT"));
}

/** OR оператор. */

TEST(OperatorsTritSetTest, OperatorORContinually) {
    TritSet setEmpty;
    TritSet setTrue(10, Trit(True)), setFalse(10, Trit(False)), setUnknown(10, Trit(Unknown));

    assertEqualSizeAndCapacity(setEmpty, 0);
    assertEqualSizeAndCapacity(setTrue, 10);
    assertEqualSizeAndCapacity(setFalse, 10);
    assertSizeAndCapacity(setUnknown, 0, 10);
    
    // Empty | Empty
    TritSet set = setEmpty | setEmpty;
    assertEqualSizeAndCapacity(set, 0);
    ASSERT_EQ(set, setEmpty);
    
    // True | Empty
    set = setTrue | setEmpty;
    assertEqualSizeAndCapacity(set, 10);
    ASSERT_EQ(set, setTrue);
    
    // Unknown | Empty
    set = setUnknown | setEmpty;
    assertEqualSizeAndCapacity(set, 0);
    ASSERT_EQ(set, setEmpty);
    
    // False | Empty
    set = setFalse | setEmpty;
    assertEqualSizeAndCapacity(set, 0);
    ASSERT_EQ(set, setEmpty);
    
    // False | False
    set = setFalse | setFalse;
    assertEqualSizeAndCapacity(set, 10);
    ASSERT_EQ(set, setFalse);
    
    // True | True
    set = setTrue | setTrue;
    assertEqualSizeAndCapacity(set, 10);
    ASSERT_EQ(set, setTrue);
    
    // True | False
    set = setTrue | setFalse;
    assertEqualSizeAndCapacity(set, 10);
    ASSERT_EQ(set, setTrue);
    
    // Unknown | Unknown
    set = setUnknown | setUnknown;
    assertEqualSizeAndCapacity(set, 0);
    ASSERT_EQ(set, setEmpty);
    
    // Unknown | True
    set = setUnknown | setTrue;
    assertEqualSizeAndCapacity(set, 10);
    ASSERT_EQ(set, setTrue);
    
    // Unknown | False
    set = setUnknown | setFalse;
    assertEqualSizeAndCapacity(set, 0);
    ASSERT_EQ(set, setEmpty);
}

TEST(OperatorsTritSetTest, OperatorORDifferent) {
    TritSet setLeft = tritSetFromString("FUT"), setRight = tritSetFromString("TFU");
    TritSet setLeftSmall = tritSetFromString("TU"), setRightSmall = tritSetFromString("UF");
    
    TritSet set = setLeft | setRight;
    assertEqualSizeAndCapacity(set, 3);
    ASSERT_EQ(set, tritSetFromString("TUT"));
    
    set = setLeft | setLeftSmall;
    assertEqualSizeAndCapacity(set, 3);
    ASSERT_EQ(set, tritSetFromString("TUT"));
    
    set = setRight | setRightSmall;
    assertEqualSizeAndCapacity(set, 2);
    ASSERT_EQ(set, tritSetFromString("TF"));
    
    set = setRight | setLeftSmall;
    assertEqualSizeAndCapacity(set, 1);
    ASSERT_EQ(set, tritSetFromString("T"));
}

/** AND оператор. */

TEST(OperatorsTritSetTest, OperatorANDContinually) {
    TritSet setEmpty;
    TritSet setTrue(10, Trit(True)), setFalse(10, Trit(False)), setUnknown(10, Trit(Unknown));
    
    assertEqualSizeAndCapacity(setEmpty, 0);
    assertEqualSizeAndCapacity(setTrue, 10);
    assertEqualSizeAndCapacity(setFalse, 10);
    assertSizeAndCapacity(setUnknown, 0, 10);
    
    // Empty & Empty
    TritSet set = setEmpty & setEmpty;
    assertEqualSizeAndCapacity(set, 0);
    ASSERT_EQ(set, setEmpty);
    
    // True & Empty
    set = setTrue & setEmpty;
    assertEqualSizeAndCapacity(set, 0);
    ASSERT_EQ(set, setEmpty);
    
    // Unknown & Empty
    set = setUnknown & setEmpty;
    assertEqualSizeAndCapacity(set, 0);
    ASSERT_EQ(set, setEmpty);
    
    // False & Empty
    set = setFalse & setEmpty;
    assertEqualSizeAndCapacity(set, 10);
    ASSERT_EQ(set, setFalse);
    
    // False & False
    set = setFalse & setFalse;
    assertEqualSizeAndCapacity(set, 10);
    ASSERT_EQ(set, setFalse);
    
    // True & True
    set = setTrue & setTrue;
    assertEqualSizeAndCapacity(set, 10);
    ASSERT_EQ(set, setTrue);
    
    // True & False
    set = setTrue & setFalse;
    assertEqualSizeAndCapacity(set, 10);
    ASSERT_EQ(set, setFalse);
    
    // Unknown & Unknown
    set = setUnknown & setUnknown;
    assertEqualSizeAndCapacity(set, 0);
    ASSERT_EQ(set, setEmpty);
    
    // Unknown & True
    set = setUnknown & setTrue;
    assertEqualSizeAndCapacity(set, 0);
    ASSERT_EQ(set, setEmpty);
    
    // Unknown & False
    set = setUnknown & setFalse;
    assertEqualSizeAndCapacity(set, 10);
    ASSERT_EQ(set, setFalse);
}

TEST(OperatorsTritSetTest, OperatorANDDifferent) {
    TritSet setLeft = tritSetFromString("FUT"), setRight = tritSetFromString("TFU");
    TritSet setLeftSmall = tritSetFromString("TU"), setRightSmall = tritSetFromString("UF");
    
    TritSet set = setLeft & setRight;
    assertEqualSizeAndCapacity(set, 2);
    ASSERT_EQ(set, tritSetFromString("FF"));
    
    set = setLeft & setLeftSmall;
    assertEqualSizeAndCapacity(set, 1);
    ASSERT_EQ(set, tritSetFromString("F"));
    
    set = setRight & setRightSmall;
    assertEqualSizeAndCapacity(set, 2);
    ASSERT_EQ(set, tritSetFromString("UF"));
    
    set = setRight & setLeftSmall;
    assertEqualSizeAndCapacity(set, 2);
    ASSERT_EQ(set, tritSetFromString("TF"));
}

/** Оператор сравнения. */

TEST(OperatorsTritSetTest, OperatorEquals) {
    TritSet setTrue(10, Trit(True)), setFalse(10, Trit(False)), setUnknown(10, Trit(Unknown));
    TritSet setLeft = tritSetFromString("FUT"), setRight = tritSetFromString("UTF"), setSmall = tritSetFromString("TF");
    
    ASSERT_TRUE(setTrue == setTrue);
    ASSERT_TRUE(setFalse == setFalse);
    ASSERT_TRUE(setUnknown == setUnknown);
    ASSERT_TRUE(setLeft == setLeft);
    ASSERT_TRUE(setSmall == setSmall);
    
    ASSERT_FALSE(setLeft == setRight);
    ASSERT_FALSE(setTrue == setFalse);
    ASSERT_FALSE(setTrue == setUnknown);
    ASSERT_FALSE(setFalse == setUnknown);
}

/** Оператор получения по индексу. */
TEST(OperatorsTritSetTest, OperatorGet) {
    TritSet set;
    
    ASSERT_EQ(set.setTrit(0, Trit(True))[0], Trit(True));
    ASSERT_EQ(set.setTrit(10, Trit(True))[10], Trit(True));
}

/** Оператор установки по индексу. */
TEST(OperatorsTritSetTest, OperatorSet) {
    /*TritSet set;
    
    set[0] = Trit(Unknown);
    ASSERT_EQ(set.size(), 0);
    ASSERT_EQ(set.capacity(), 0);
    
    set[10] = Trit(Unknown);
    ASSERT_EQ(set.size(), 0);
    ASSERT_EQ(set.capacity(), 0);
    
    set.[0] = Trit(True);
    ASSERT_EQ(set.capacity(), getCapacity(1));
    ASSERT_EQ(set.size(), 1);
    
    set[10] = Trit(True);
    ASSERT_EQ(set.capacity(), getCapacity(10));
    ASSERT_EQ(set.size(), 10);*/
}
