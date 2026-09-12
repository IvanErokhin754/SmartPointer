#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <stdexcept>

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> array;

    virtual ArraySequence<T>* CreateFromArray(const DynamicArray<T>& other) const = 0;
    
    void AppendInternal(const T& value) {
        array.PushBack(value);
    }

    void PrependInternal(const T& value) {
        array.Resize(array.GetSize() + 1);

        for (size_t i = array.GetSize() - 1; i > 0; --i)
            array[i] = array[i - 1];

        array[0] = value;
    }

    void InsertAtInternal(size_t index, const T& value) {
        if (index > array.GetSize())
            throw std::out_of_range("Index out of range");

        array.Resize(array.GetSize() + 1);
        for (size_t i = array.GetSize() - 1; i > index; --i) {
            array[i] = array[i - 1];
        }
        array[index] = value;
    }

    void RemoveAtInternal(size_t index) {
        if (index >= array.GetSize())
            throw std::out_of_range("Index out of range");
        
        for (size_t i = index; i < array.GetSize() - 1; i++) {
            array[i] = array[i + 1];
        }
        array.Resize(array.GetSize() - 1);
    }

public:
    class Iterator {
        private:
            const ArraySequence<T>* seq;
            size_t index;
        public:
            Iterator(const ArraySequence<T>* seq, size_t index) : seq(seq), index(index) {}
            
            const T& operator*() const {
                return seq->Get(index);
            }

            Iterator& operator++() {
                ++index;
                return *this;
            }

            bool operator!=(const Iterator& other) const {
                return index != other.index || seq != other.seq;
            }

            bool operator==(const Iterator& other) const {
                return index == other.index && seq == other.seq;
            }
    };

    ArraySequence() : array() {}
    ArraySequence(T *items, int count) : array(items, count) {}
    ArraySequence(const DynamicArray<T> &other) : array(other) {}
    ArraySequence(const ArraySequence<T> &other) : array(other.array) {} 
    
    ArraySequence<T>& operator=(const ArraySequence<T>& other) {
        if (this == &other)
            return *this;
        
        array = other.array;
        return *this;
    }

    Iterator begin() const {
        return Iterator(this, 0);
    }

    Iterator end() const {
        return Iterator(this, GetLength());
    }

    const T& operator[](size_t index) const {
        return Get(index);
    }

    const T& Get(size_t index) const override {
        if (index >= array.GetSize()) 
            throw std::out_of_range("Index out of range");
        return array[index];
    }

    void Set(size_t index, const T& value) override {
        if (index >= array.GetSize())
            throw std::out_of_range("Index out of range");
        
        array[index] = value;
    }

    const T& GetFirst() const override {
        if (array.GetSize() == 0)
            throw std::out_of_range("Sequence is empty");
        return array[0];
    }
    
    const T& GetLast() const override {
        if (array.GetSize() == 0)
            throw std::out_of_range("Sequence is empty");
        return array[array.GetSize() - 1];
    }

    size_t GetLength() const override {
        return array.GetSize();
    }

    Sequence<T>* GetSubsequence(size_t start_index, size_t end_index) const override {
        if (start_index > end_index || end_index >= array.GetSize())
            throw std::out_of_range("Index out of range");
        
        size_t length= end_index - start_index + 1;
        DynamicArray<T> temp(length);
        
        for (size_t i = 0; i < length; i++) {
            temp[i] = array[start_index + i];
        }

        return CreateFromArray(temp);
    }

    Sequence<T>* Map(T (*func)(const T&)) const {
        DynamicArray<T> temp(this->GetLength());
        
        for (size_t i = 0; i < this->GetLength(); i++) {
            temp[i] = func(this->Get(i));
        }

        return CreateFromArray(temp);
    }

    Sequence<T>* Where(bool (*pred)(const T&)) const {
        DynamicArray<T> temp;
        
        for (size_t i = 0; i < this->GetLength(); i++) {
            if (pred(this->Get(i))) {
                temp.PushBack(this->Get(i));
            }
        }

        return CreateFromArray(temp);
    }

    Sequence<T>* Concat(const Sequence<T>& other) const override {
        DynamicArray<T> temp(this->GetLength() + other.GetLength());
        for (size_t i = 0; i < this->GetLength(); i++) {
            temp[i] = this->Get(i);
        }

        for (size_t i = 0; i < other.GetLength(); i++) {
            temp[this->GetLength() + i] = other.Get(i);
        }

        return CreateFromArray(temp);
    }
};
#endif /* ARRAY_SEQUENCE_HPP */
