#pragma once
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <string>
#include "simple_vector.h"

// У функции, объявленной со спецификатором inline, может быть несколько
// идентичных определений в разных единицах трансляции.
// Обычно inline помечают функции, чьё тело находится в заголовочном файле,
// чтобы при подключении этого файла из разных единиц трансляции не возникало ошибок компоновки
inline void Test1() {
    // Инициализация конструктором по умолчанию
    {
        SimpleVector<int> v;
        assert(v.GetSize() == 0u);
        assert(v.IsEmpty());
        assert(v.GetCapacity() == 0u);
    }

    // Инициализация вектора указанного размера
    {
        SimpleVector<int> v(5);
        assert(v.GetSize() == 5u);
        assert(v.GetCapacity() == 5u);
        assert(!v.IsEmpty());
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 0);
        }
    }

    // Инициализация вектора, заполненного заданным значением
    {
        SimpleVector<int> v(3, 42);
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 42);
        }
    }

    // Инициализация вектора при помощи initializer_list
    {
        SimpleVector<int> v{1, 2, 3};
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        assert(v[2] == 3);
    }

    // Доступ к элементам при помощи At
    {
        SimpleVector<int> v(3);
        assert(&v.At(2) == &v[2]);
        try {
            v.At(3);
            assert(false);  // Ожидается выбрасывание исключения
        } catch (const std::out_of_range&) {
        } catch (...) {
            assert(false);  // Не ожидается исключение, отличное от out_of_range
        }
    }

    // Очистка вектора
    {
        SimpleVector<int> v(10);
        const size_t old_capacity = v.GetCapacity();
        v.Clear();
        assert(v.GetSize() == 0);
        assert(v.GetCapacity() == old_capacity);
    }

    // Изменение размера
    {
        SimpleVector<int> v(3);
        v[2] = 17;
        v.Resize(7);
        assert(v.GetSize() == 7);
        assert(v.GetCapacity() >= v.GetSize());
        assert(v[2] == 17);
        assert(v[3] == 0);
    }
    {
        SimpleVector<int> v(3);
        v[0] = 42;
        v[1] = 55;
        const size_t old_capacity = v.GetCapacity();
        v.Resize(2);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() == old_capacity);
        assert(v[0] == 42);
        assert(v[1] == 55);
    }
    {
        const size_t old_size = 3;
        SimpleVector<int> v(3);
        v.Resize(old_size + 5);
        v[3] = 42;
        v.Resize(old_size);
        v.Resize(old_size + 2);
        assert(v[3] == 0);
    }

    // Итерирование по SimpleVector
    {
        // Пустой вектор
        {
            SimpleVector<int> v;
            assert(v.begin() == nullptr);
            assert(v.end() == nullptr);
        }

        // Непустой вектор
        {
            SimpleVector<int> v(10, 42);
            assert(v.begin());
            assert(*v.begin() == 42);
            assert(v.end() == v.begin() + v.GetSize());
        }
    }
    // Конструктор копирования
    {
        {
            SimpleVector<int> v{1, 2, 3};
            SimpleVector<int> copy(v);
            assert(copy.GetSize() == 3);
            assert(copy.GetCapacity() == 3);
            assert(copy[2] == 3);
        }
    }

    //Функция Swap
    {
        SimpleVector<int> v{1, 2, 3};
        SimpleVector<int> copy{4, 5, 6, 7};
        copy.swap(v);
        assert(copy.GetSize() == 3);
        assert(copy.GetCapacity() == 3);
        assert(copy[2] == 3);
        assert(v.GetSize() == 4);
        assert(v.GetCapacity() == 4);
        assert(v[2] == 6);
    }

    //Оператор присваивания
    {
        {
            SimpleVector<int> v{1, 2, 3};
            SimpleVector<int> copy{4, 5, 6, 7};
            copy = v;
            assert(copy.GetSize() == 3);
            assert(copy.GetCapacity() == 3);
            assert(copy[2] == 3);
        }
    }

    //Функция PushBack
    {
        SimpleVector<int> v{1, 2, 3};
        v.PushBack(5);
        assert(v.GetSize() == 4);
        assert(v.GetCapacity() >= 4);
        assert(v[3] == 5);
    }

    //Функция PopBack
    {
        SimpleVector<int> v{1, 2, 3, 5};
        v.PopBack();
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() >= 3);
        assert(*(v.end()-1) == 3);
    }

    //Функция Erase
    {
        SimpleVector<int> v{1, 2, 3, 5};
        v.Erase(v.begin()+1);
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() >= 3);
        assert(*(v.begin()+1) == 3);
    }

    //Функция Erase
    {
        SimpleVector<int> v{1, 2, 3, 5};
        v.Insert(v.begin()+1, 4);
        assert(v.GetSize() == 5);
        assert(v.GetCapacity() >= 5);
        assert(*(v.begin()+1) == 4);
        assert(v.At(2) == 2);
        assert(v.At(3) == 3);
        assert(v.At(4) == 5);
    }
}

inline void Test2() {
    // PushBack
    {
        SimpleVector<int> v(1);
        v.PushBack(42);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() >= v.GetSize());
        assert(v[0] == 0);
        assert(v[1] == 42);
    }

    // Если хватает места, PushBack не увеличивает Capacity
    {
        SimpleVector<int> v(2);
        v.Resize(1);
        const size_t old_capacity = v.GetCapacity();
        v.PushBack(123);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() == old_capacity);
    }

    // PopBack
    {
        SimpleVector<int> v{0, 1, 2, 3};
        const size_t old_capacity = v.GetCapacity();
        const auto old_begin = v.begin();
        v.PopBack();
        assert(v.GetCapacity() == old_capacity);
        assert(v.begin() == old_begin);
        assert((v == SimpleVector<int>{0, 1, 2}));
    }

    // Конструктор копирования
    {
        SimpleVector<int> numbers{1, 2};
        auto numbers_copy(numbers);
        assert(&numbers_copy[0] != &numbers[0]);
        assert(numbers_copy.GetSize() == numbers.GetSize());
        for (size_t i = 0; i < numbers.GetSize(); ++i) {
            assert(numbers_copy[i] == numbers[i]);
            assert(&numbers_copy[i] != &numbers[i]);
        }
    }

    // Сравнение
    {
        assert((SimpleVector{1, 2, 3} == SimpleVector{1, 2, 3}));
        assert((SimpleVector{1, 2, 3} != SimpleVector{1, 2, 2}));

        assert((SimpleVector{1, 2, 3} < SimpleVector{1, 2, 3, 1}));
        assert((SimpleVector{1, 2, 3} > SimpleVector{1, 2, 2, 1}));

        assert((SimpleVector{1, 2, 3} >= SimpleVector{1, 2, 3}));
        assert((SimpleVector{1, 2, 4} >= SimpleVector{1, 2, 3}));
        assert((SimpleVector{1, 2, 3} <= SimpleVector{1, 2, 3}));
        assert((SimpleVector{1, 2, 3} <= SimpleVector{1, 2, 4}));
    }

    // Обмен значений векторов
    {
        SimpleVector<int> v1{42, 666};
        SimpleVector<int> v2;
        v2.PushBack(0);
        v2.PushBack(1);
        v2.PushBack(2);
        const int* const begin1 = &v1[0];
        const int* const begin2 = &v2[0];

        const size_t capacity1 = v1.GetCapacity();
        const size_t capacity2 = v2.GetCapacity();

        const size_t size1 = v1.GetSize();
        const size_t size2 = v2.GetSize();

        static_assert(noexcept(v1.swap(v2)));
        v1.swap(v2);
        assert(&v2[0] == begin1);
        assert(&v1[0] == begin2);
        assert(v1.GetSize() == size2);
        assert(v2.GetSize() == size1);
        assert(v1.GetCapacity() == capacity2);
        assert(v2.GetCapacity() == capacity1);
    }

    // Присваивание
    {
        SimpleVector<int> src_vector{1, 2, 3, 4};
        SimpleVector<int> dst_vector{1, 2, 3, 4, 5, 6};
        dst_vector = src_vector;
        assert(dst_vector == src_vector);
    }

    // Вставка элементов
    {
        SimpleVector<int> v{1, 2, 3, 4};
        v.Insert(v.begin() + 2, 42);
        assert((v == SimpleVector<int>{1, 2, 42, 3, 4}));
    }

    // Удаление элементов
    {
        SimpleVector<int> v{1, 2, 3, 4};
        v.Erase(v.cbegin() + 2);
        assert((v == SimpleVector<int>{1, 2, 4}));
    }
}

inline void Test3()
{
    using namespace std::literals;
    {
        std::cout << "TestReserveConstructor"s << std::endl;
        SimpleVector<int> v(Reserve(5));
        assert(v.GetCapacity() == 5);
        assert(v.IsEmpty());
        std::cout << "Done!"s << std::endl;
    }
    
    {
        std::cout << "TestReserveMethod"s << std::endl;
        SimpleVector<int> v;
        // зарезервируем 5 мест в векторе
        v.Reserve(5);
        assert(v.GetCapacity() == 5);
        assert(v.IsEmpty());

        // попытаемся уменьшить capacity до 1
        v.Reserve(1);
        // capacity должно остаться прежним
        assert(v.GetCapacity() == 5);
        // поместим 10 элементов в вектор
        for (int i = 0; i < 10; ++i) {
            v.PushBack(i);
        }
        assert(v.GetSize() == 10);
        // увеличим capacity до 100
        v.Reserve(100);
        // проверим, что размер не поменялся
        assert(v.GetSize() == 10);
        assert(v.GetCapacity() == 100);
        // проверим, что элементы на месте
        for (int i = 0; i < 10; ++i) {
            assert(v[i] == i);
        }
        std::cout << "Done!"s << std::endl;
    }
}

inline void Test4() {
    /*struct NoCopyStruct
    {
        int number_;
        
        NoCopyStruct(const NoCopyStruct&) = delete;
        NoCopyStruct& operator=(const NoCopyStruct&) = delete;

        NoCopyStruct(NoCopyStruct&&) = default;
        NoCopyStruct& operator=(NoCopyStruct&&) = default;
    };*/

    {
        SimpleVector<int> v({3, 42});
        SimpleVector<int> v_for_move;
        assert(v.GetSize() == 2);
        assert(v_for_move.GetSize() == 0);
        v_for_move = std::move(v);
        assert(v_for_move.GetSize() == 2);
        assert(v_for_move.At(0) == 3);
        assert(v_for_move.At(1) == 42);
    }

    {
        SimpleVector<int> v({3, 42});
        SimpleVector<int> v_for_move(std::move(v));
        assert(v_for_move.GetSize() == 2);
        assert(v_for_move.At(0) == 3);
        assert(v_for_move.At(1) == 42);
    }
}