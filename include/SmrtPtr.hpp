#ifndef SMART_PTR_HPP
#define SMART_PTR_HPP

#include <stdexcept>
#include <type_traits>
#include "MutableArraySequence.hpp"

enum class PointerType {
    Single,
    Array
};

struct Storage {
    void *ptr;
    size_t counter;

    size_t size;
    PointerType is_array;

    void (*deleter)(void*);
};

template<typename U>
void DeleteSingle(void *ptr) {
    delete static_cast<U*>(ptr);
}

template<typename U>
void DeleteArray(void *ptr) {
    delete[] static_cast<U*>(ptr);
}

inline size_t global_storage_next_id = 0;

inline MutableArraySequence<Storage> global_storage;

template<typename T>
class SmrtPtr {
private:
    T *ptr;
    size_t id;

    void ReleaseReference() {
        if (ptr == nullptr)
            return;
        
        global_storage[id].counter--;

        if (global_storage[id].counter == 0) {
            global_storage[id].deleter(global_storage[id].ptr);
            global_storage[id].ptr = nullptr;
        }

        ptr = nullptr;
    }

public:
    SmrtPtr() : ptr(nullptr), id(0) {}
    SmrtPtr(SmrtPtr&& other) noexcept : ptr(other.ptr), id(other.id) {
        other.ptr = nullptr;
    }
    SmrtPtr(const SmrtPtr &other) : ptr(other.ptr), id(other.id) {
        if (ptr != nullptr)
            global_storage[id].counter++;
    }
    SmrtPtr(T *other, size_t size) : ptr(other), id(0) {
        if (other != nullptr) {
            id = global_storage_next_id++;
            global_storage.Append({ptr, 1, size, PointerType::Array, &DeleteArray<T>});
        }   
    }
    SmrtPtr(T *other) : ptr(other), id(0) {
        if (other != nullptr) {
            id = global_storage_next_id++;
            global_storage.Append({ptr, 1, 1, PointerType::Single, &DeleteSingle<T>});
        }
    }

    template<typename U>
    requires std::is_convertible_v<U*, T*>
    SmrtPtr(const SmrtPtr<U>& other) : ptr(other.ptr), id(other.id) {
        if (ptr != nullptr)
            global_storage[id].counter++;
    }

    template<typename U>
    requires std::is_convertible_v<U*, T*>
    SmrtPtr(SmrtPtr<U>&& other) : ptr(other.ptr), id(other.id) {
        other.ptr = nullptr;
    }

    SmrtPtr& operator=(const SmrtPtr &other) {
        if (&other == this) {
            return *this;
        }
        
        ReleaseReference();

        if (other.ptr != nullptr)
            global_storage[other.id].counter++;

        ptr = other.ptr;
        id = other.id;

        return *this;
    }

    SmrtPtr& operator=(SmrtPtr&& other) noexcept {
        if (&other == this) {
            return *this;   
        }

        ReleaseReference();

        ptr = other.ptr;
        id = other.id;

        other.ptr = nullptr;

        return *this;
    }

    template<typename U>
    friend class SmrtPtr;

    template<typename U>
    requires std::is_convertible_v<U*, T*>
    SmrtPtr& operator=(const SmrtPtr<U>& other) {
        ReleaseReference();

        if (other.ptr != nullptr)
            global_storage[other.id].counter++;
        
        ptr = other.ptr;
        id = other.id;

        return *this;
    }

    template<typename U>
    requires std::is_convertible_v<U*, T*>
    SmrtPtr& operator=(SmrtPtr<U>&& other) noexcept {    
        ReleaseReference();

        ptr = other.ptr;
        id = other.id;

        other.ptr = nullptr;

        return *this;
    }

    T& operator[](size_t index) {
        if (ptr == nullptr)
            throw std::logic_error("Operator[] : pointer is null");
        if (global_storage[id].is_array == PointerType::Single)
            throw std::logic_error("Operator[] : pointer is not array");
        if (index >= global_storage[id].size)
            throw std::out_of_range("Operator[] : index out of range");
        
        return *(ptr + index);
    }

    T& operator*() {
        if (ptr == nullptr)
            throw std::logic_error("ptr is null");
        if (global_storage[id].is_array == PointerType::Array)
            throw std::logic_error("ptr is array");
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }

    T* Get() const {
        return ptr;
    }

    operator bool() const {
        return ptr != nullptr;
    }

    ~SmrtPtr() {
        ReleaseReference();
    }

    size_t UseCount() const {
        if (ptr != nullptr)
            return global_storage[id].counter;
        return 0;
    }
};


#endif /* SMART_PTR_HPP */
