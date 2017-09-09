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
#include <unordered_map>

// Все ради одного замечательного компилятора...
#ifdef _MSC_VER
typedef unsigned int uint;
#endif

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
    
    class ModifiableTrit;
    
    /**
     * @param tritsCount Кол-во тритов, которые нужно изначально выделить.
     * Значение памяти округляется в большую сторону, то есть ceil(tritsCount * 2 / 8. / sizeof(uint))
     * @param defaultValue Чем изначально заполнить выделенные триты.
     */
    TritSet(size_t tritsCount, Trit defaultValue);
    
    /**
     * @see TritSet(size_t, Trit)
     */
    TritSet(size_t tritsCount) : TritSet(tritsCount, Trit(Unknown)) {}
    
    /**
     * @see TrirSet(size_t, Trit)
     */
    TritSet() : TritSet(0) {}
    
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
     * Подсчитывает кол-во установленных в данное значение тритов.
     * Для трита Unknown - кол-во тритов Unknown до последнего
     * установленного Unknown трита.
     * @param trit Тип трита.
     * @return Кол-во тритов.
     */
    size_t cardinality(Trit trit) const;
    
    /**
     * Подсчитывает кол-во установленных в данное значение тритов.
     * Для трита Unknown - кол-во тритов Unknown до последнего
     * установленного Unknown трита.
     * @return Кол-во тритов каждого из типов.
     */
    std::unordered_map<Trit, size_t, std::hash<size_t>> cardinality() const;
    
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
    ModifiableTrit operator[](size_t pos) const;
    
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
    std::ostream& operator<<(std::ostream& stream);
    
    /** Для выражений set[%index%] и set[%index%] = %value% */
    class ModifiableTrit {
    public:
        ModifiableTrit& operator=(Trit trit) {
            set.setTrit(pos, trit);
            return *this;
        }
        
        operator Trit() const {
            return set.getTrit(pos);
        }
        
    private:
        size_t pos;
        TritSet& set;
        
        ModifiableTrit(TritSet& set, const size_t pos): set(set), pos(pos) {}
        
        ModifiableTrit(const ModifiableTrit& trit): set(trit.set), pos(trit.pos) {}
        
        ModifiableTrit& operator=(ModifiableTrit const&) = delete;
    
        friend TritSet;
    };
    
private:
    size_t lastTritPos; // Позиция последнего не Unknown трита
    
    std::vector<uint> storage;
    
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

/** Тритовые операции. */

Trit operator~(const Trit& trit);
Trit operator&(const Trit& left, const Trit& right);
Trit operator|(const Trit& left, const Trit& right);

#endif /* TritSet_h */
