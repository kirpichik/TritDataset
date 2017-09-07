//
//  TritSet.h
//  TritDataset
//
//  Created by Кирилл on 06.09.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef TritSet_h
#define TritSet_h

#include <iostream>
#include <vector>

/**
 * Возможные значения тритов.
 */
enum Trit {
    False,
    Unknown,
    True
};

class TritSet {
public:
    
    /**
     * @param tritsCount Кол-во тритов, которые нужно изначально выделить.
     * Значение памяти округляется в большую сторону, то есть ceil(tritsCount * 2 / 8. / sizeof(uint))
     * @param defaultValue Чем изначально заполнить выделенные триты.
     */
    TritSet(size_t tritsCount, Trit defaultValue);
    
    /**
     * @see TritSet(size_t, Trit)
     */
    TritSet(size_t tritsCount) {
        TritSet(tritsCount, Trit(Unknown));
    }
    
    /**
     * @see TrirSet(size_t, Trit)
     */
    TritSet() {
        TritSet(0);
    }
    
    /**
     * Размер текущей выделенной памяти для тритов в байтах.
     * @return Размер выделенной для тритов памяти в байтах.
     */
    size_t capacity() const;
    
    /**
     * Размер набора тритов.
     * @return Индекс последнего установленного не Unknown трита + 1.
     */
    size_t size() const;
    
    /**
     * Получает значение трита для данной позиции.
     * @param pos Позиция для получения трита.
     * @return Значение трита на данной позиции.
     */
    Trit getTrit(size_t pos) const;
    
    /**
     * Удаляет все значения после позиции from и освобождает лишнюю память.
     * @param from Позиция удаления.
     * @return Измененный объект(самого себя)
     */
    TritSet& trim(size_t from);
    
    /**
     * Освобождает "лишнюю" память до последнего не Unknown трита.
     * @return Измененный объект(самого себя)
     */
    TritSet& shrink();
    
    /** Устанавливает трит на заданную позицию.
     * Если позиция трита выходит за рамки выделенной памяти
     * и новое значение равно True или False, то все триты идущие до
     * последнего выделенного значения и не равные True или False
     * устанавливаются в Unknown.
     * Если позиция трита выходит за рамки выделенной памяти
     * и новое значение равно Unknown, память не выделяется.
     *
     * @param pos Позиция установки.
     * @param value Устанавлимое значение.
     * @return Измененный объект(самого себя)
     */
    TritSet& setTrit(size_t pos, Trit value);
    
    /**
     * Оператор сравнения.
     */
    bool operator==(const TritSet& set) const;
    
    /**
     * Отрицание оператора сравнения.
     */
    bool operator!=(const TritSet& set) const;
    
    /**
     * Получение трита по индексу.
     * */
    Trit operator[](size_t pos) const;
    
    /**
     * Логическое NOT.
     */
    TritSet operator~() const;
    
    /**
     * Логическое AND.
     */
    TritSet operator&(const TritSet& set) const;
    
    /**
     * Логическое OR.
     */
    TritSet operator|(const TritSet& set) const;
    
    /**
     * Вывод в поток.
     */
    std::ostream& operator<<(std::ostream& stream) const;
    
private:
    size_t lastTritPos; // Позиция последнего не Unknown трита
    
    std::vector<uint> storage;
    
    // Счетчики значений тритов.
    size_t falseTritCount;
    size_t unknownTritCount;
    size_t trueTritCount;
    
    /** Устанавливает трит на заданную позицию.
     * Если позиция трита выходит за рамки выделенной памяти
     * и новое значение равно True или False, то все триты идущие до
     * последнего выделенного значения и не равные True или False
     * устанавливаются в Unknown.
     * Так же не обновляет позицию последенго не Unknown трита.
     *
     * @param pos Позиция установки.
     * @param value Устанавлимое значение.
     */
    void _setTrit(size_t pos, Trit value);
    
    /**
     * Подсчитывает позицию последнего не Unkwnown трита.
     */
    void countLastTritPos();
};

#endif /* TritSet_h */
