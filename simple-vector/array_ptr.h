#pragma once

#include <stdexcept>
#include <algorithm>

template <typename T>
class ArrayPtr
{
    public:
    ArrayPtr() = default;
    
    explicit ArrayPtr(T* raw_ptr) noexcept
        : ptr_(raw_ptr) {

    }

    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr()
    {
        delete[] ptr_;
    }

    T* GetRawPtr() const noexcept
    {
        return ptr_;
    }

    T* Release() noexcept
    {
        T* p = ptr_;
        ptr_ = nullptr;
        return p;
    }

    explicit operator bool() const
    {
        return ptr_ != nullptr;
    }

    ArrayPtr& operator=(ArrayPtr& rhs)
    {
        if(ptr_ != rhs.GetRawPtr())
        {
            this->ptr_ = rhs.GetRawPtr();
            rhs.Release();
        }
        return *this;
    }

    void swap(ArrayPtr& rhs)
    {
        std::swap(ptr_, rhs.ptr_);
    }

    T* operator->() const
    {
        using namespace std::literals;
        return !ptr_ ? throw std::logic_error("ArrayPtr is null"s) : ptr_;
    }

    T& operator*() const
    {
        using namespace std::literals;
        return !ptr_ ? throw std::logic_error("ArrayPtr is null"s) : *ptr_;
    }

    private:
    T* ptr_ = nullptr;
};