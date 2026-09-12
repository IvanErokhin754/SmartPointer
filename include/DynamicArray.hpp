#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <cstddef>
#include <stdexcept>

template<typename T>
class DynamicArray{    
private:
    size_t size;
    size_t capacity;
    T *data;

    static T* AllocateAndCopy(const T* source, size_t count) {
        if (!count)
            return nullptr;
        
        T* new_data = new T[count];
        for (size_t i = 0; i < count; i++) {
            new_data[i] = source[i];
        }
        return new_data;
    }

public:
    DynamicArray() : size(0), capacity(0), data(nullptr) {}
    DynamicArray(size_t size) : size(size), capacity(size) {
        data = (size == 0) ? nullptr : new T[size];
    }  
    DynamicArray(const DynamicArray &other) : size(other.size), capacity(other.size), data(AllocateAndCopy(other.data, other.size)) {}
    DynamicArray(T* items, int count) : size(count), capacity(count) {
        data = (count == 0) ? nullptr : new T[count];
        for (int i = 0; i < count; i++) {
            data[i] = items[i];
        }
    }
    ~DynamicArray() {
        delete[] data;
    }
    
    T &operator[](size_t i) {
        if (i >= size)
            throw std::out_of_range("Index larger than size!");
        
        return data[i];
    }
    
    const T &operator[](size_t i) const {
        return const_cast<DynamicArray<T>*>(this)->operator[](i);
    }

    DynamicArray &operator=(const DynamicArray &other) { 
        if (this == &other)
            return *this;

        T *new_data = AllocateAndCopy(other.data, other.size);

        delete[] data;
        data = new_data;
        size = other.size;
        capacity = other.size;

        return *this;
    }

    void PushBack(const T& value) {
        Resize(size + 1);
        data[size - 1] = value;
    }

    size_t GetCapacity() const {
        return capacity;
    }
    void Resize(size_t new_size) {
        if (new_size == size) return;
        if (new_size == 0) {
            size = 0;
            return;
        }

        if (new_size <= capacity) {
            size = new_size;
            return;
        }

        size_t new_capacity = (capacity == 0) ? 1 : capacity;
        while (new_capacity < new_size) {
            new_capacity *= 2;
        }

        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size; i++) {
            new_data[i] = data[i];
        }

        delete[] data;
        data = new_data;
        capacity = new_capacity;
        size = new_size;
    }   

    size_t GetSize() const {
        return size;
    }

};

#endif /* DYNAMIC_ARRAY_HPP */
