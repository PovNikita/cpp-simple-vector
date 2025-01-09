#pragma once

#include <stdexcept>
#include <algorithm>
#include <utility>

template <typename T>
class ArrayPtr
{
    public:
    ArrayPtr() = default;

    ArrayPtr(const ArrayPtr&) = delete;
    ArrayPtr& operator=(ArrayPtr&) = delete;
    
    explicit ArrayPtr(T* raw_ptr) noexcept
        : ptr_(raw_ptr) {

    }

    ArrayPtr(ArrayPtr&& other)
    {
        std::swap(this->ptr_, other.ptr_);
    }

    ~ArrayPtr()
    {
        delete[] ptr_;
    }

    ArrayPtr& operator=(ArrayPtr&& rhs)
    {
        if(this->ptr_ == rhs.ptr_)
        {
            return *this;
        }

        std::swap(this->ptr_, rhs.ptr_);
        return *this;
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



    void swap(ArrayPtr& rhs)
    {
        std::swap(ptr_, rhs.ptr_);
    }

    T* operator->() const noexcept
    {
        return ptr_;
    }

    T& operator*() const noexcept
    {
        return *ptr_;
    }

    private:
    T* ptr_ = nullptr;
};