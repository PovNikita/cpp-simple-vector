#pragma once

#include <cassert>
#include <initializer_list>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <functional>
#include "array_ptr.h"

class SaveReserve
{
public:
    SaveReserve(size_t capacity)
    : capacity_(capacity)
    {}
    size_t GetCapacity() const
    {
        return capacity_;
    }
private:
    size_t capacity_ = 0;
};

SaveReserve Reserve(size_t capacity)
{
    return SaveReserve(capacity);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size){
        if(size == 0)
        {
            return;
        }
        try
        {
            ArrayPtr<Type> ptr(new Type[size]());
            vector_ = ptr;
            capacity_ = size;
            size_ = size;
        }
        catch(...)
        {
            throw;
        }

    }

    //конструктор копирования
    explicit SimpleVector(const SimpleVector& other) {   
        if(other.GetSize() == 0)
        {
            return;
        }
        try
        {
            size_t capacity = other.GetCapacity();
            size_t size = other.GetSize();
            ArrayPtr<Type> ptr(new Type[capacity]());
            vector_ = ptr;
            size_ = size;
            capacity_ = capacity;
            for(size_t i = 0; i < size; ++i)
            {
                vector_.GetRawPtr()[i] = other.At(i);
            }
        }
        catch(...)
        {
            throw;
        }
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) {
        if(size == 0)
        {
            return;
        }
        try
        {
            ArrayPtr<Type> ptr(new Type[size]());
            vector_ = ptr;
            capacity_ = size;
            size_ = size;
            for(size_t i = 0; i < size; ++i)
            {
                vector_.GetRawPtr()[i] = value;
            }
        }
        catch(...)
        {
            throw;
        }
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        if(init.size() == 0)
        {
            return;
        }
        try
        {
            size_t capacity = init.size();
            ArrayPtr<Type> ptr(new Type[capacity]());
            vector_ = ptr;
            capacity_ = capacity;
            size_ = capacity;
            for(size_t i = 0; i < init.size(); ++i)
            {
                vector_.GetRawPtr()[i] = init.begin()[i];
            }
        }
        catch(...)
        {
            throw;
        }
    }

    //Конструктор перемещения
    SimpleVector(SimpleVector &&other)
    {
        vector_.swap(other.vector_);
        std::swap(this->size_, other.size_);
        std::swap(this->capacity_, other.capacity_);
    }

    size_t ReserveFunc(size_t new_capacity)
    {
        return new_capacity;
    }

    //Резервирующий конструктор
    SimpleVector(const SaveReserve &res)
    {
        ArrayPtr<Type> ptr;
        vector_ = ptr;
        size_ = 0;
        capacity_ = 0;
        Reserve(res.GetCapacity());
    }

    ~SimpleVector()
    {

    }

    void Reserve(size_t new_capacity)
    {
        if(new_capacity > capacity_)
        {
            try{
                ArrayPtr<Type> temp(new Type[new_capacity]);
                std::copy(begin(), end(), temp.GetRawPtr());
                std::fill(std::next(temp.GetRawPtr(), static_cast<int>(size_)), std::next(temp.GetRawPtr(), static_cast<int>(new_capacity)), Type());
                vector_ = temp;
                capacity_ = new_capacity;
            }
            catch(...)
            {
                throw;
            }
        }
        else
        {
            return;
        }
    }

    SimpleVector<Type>& operator=(const SimpleVector& rhs)
    {
        if(begin() != rhs.begin())
        {
            SimpleVector<Type> rhs_copy(rhs);
            swap(rhs_copy);
        }
        return *this;
    }

    SimpleVector<Type>& operator=(SimpleVector&& other)
    {
        if(this == &other)
        {
            return *this;
        }
        vector_.swap(other.vector_);
        std::swap(this->size_, other.size_);
        std::swap(this->capacity_, other.capacity_);
        return *this;
    }

    void swap(SimpleVector& other) noexcept
    {
        vector_.swap(other.vector_);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ ? false : true;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return vector_.GetRawPtr()[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        const Type& link = vector_.GetRawPtr()[index];
        return link;
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if(index >= size_)
        {
            throw std::out_of_range("");
        }
        return vector_.GetRawPtr()[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if(index >= size_)
        {
            throw std::out_of_range("");
        }
        const Type& link = vector_.GetRawPtr()[index];
        return link;
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if(new_size > capacity_)
        {
            try{
                SimpleVector<Type> temp(new_size * 2);
                std::move(begin(), end(), temp.begin());
                //std::fill(std::next(temp.GetRawPtr(), static_cast<int>(size_)), std::next(temp.GetRawPtr(), static_cast<int>(new_size*2)), Type());
                swap(temp);
                capacity_ = new_size * 2;
                size_ = new_size;
            }
            catch(...)
            {
                throw;
            }
        }
        else if(new_size > size_ && new_size <= capacity_)
        {
            std::fill(std::next(begin(), static_cast<int>(size_)), std::next(begin(), static_cast<int>(new_size)), std::move(Type()));
            size_ = new_size;
        }
        else
        {
            size_ = new_size;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return vector_.GetRawPtr();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        if(vector_.GetRawPtr() == nullptr)
        {
            return nullptr;
        }
        return &vector_.GetRawPtr()[size_];
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        const Type* ptr = vector_.GetRawPtr();
        return ptr;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        if(vector_.GetRawPtr() == nullptr)
        {
            return nullptr;
        }
        const Type* ptr = &(vector_.GetRawPtr()[size_]);
        return ptr;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        const Type* const ptr = vector_.GetRawPtr();
        return ptr;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        if(vector_.GetRawPtr() == nullptr)
        {
            return nullptr;
        }
        const Type* const ptr = &(vector_.GetRawPtr()[size_]);
        return ptr;
    }

    void PushBack(const Type& element)
    {
        size_t index = size_;
        this->Resize(size_+1);
        this->At(index) = element;
    }

    void PushBack(Type&& element)
    {
        size_t index = size_;
        this->Resize(size_+1);
        this->At(index) = std::move(element);
    }

    void PopBack() noexcept
    {
        --size_;
    }

    Iterator Erase(ConstIterator pos)
    {
        long int index = std::distance(cbegin(), pos);
        size_t i = static_cast<size_t>(index);
        for( ;i < size_- 1; ++i)
        {
            this->At(i) = std::move(this->At(i + 1));
        }
        --size_;
        Iterator it = std::next(begin(), index);
        return it;
    }

    Iterator Insert(ConstIterator pos, const Type& value)
    {
        auto distance = std::distance(cbegin(), pos);
        size_t size_before = size_;
        Resize(size_ + 1);
        Iterator iter = begin() + distance;
        std::copy_backward(iter, begin() + size_before, end());
        *iter = value;
        return iter;
    }

    Iterator Insert(ConstIterator pos, Type&& value)
    {
        auto distance = std::distance(cbegin(), pos);
        size_t size_before = size_;
        Resize(size_ + 1);
        Iterator iter = begin() + distance;
        std::copy_backward(iter, begin() + size_before, end());
        std::swap(*iter, value);
        return iter;
    }

    

private:
    ArrayPtr<Type> vector_;
    size_t size_ = 0;
    size_t capacity_ = 0;
};


template <typename Type>
bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if(lhs.GetSize() == rhs.GetSize())
    {
        return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
    }
    return false;
}

template <typename Type>
bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename Type>
bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(rhs.cbegin(), rhs.cend(), lhs.cbegin(), lhs.cend());
}

template <typename Type>
bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs<rhs);
}
