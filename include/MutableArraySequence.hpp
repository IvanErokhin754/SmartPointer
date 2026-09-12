#ifndef MUTABLE_ARRAY_SEQUENCE_HPP
#define MUTABLE_ARRAY_SEQUENCE_HPP

#include "ArraySequence.hpp"

template<typename T>
class MutableArraySequence : public ArraySequence<T> {
protected:
    ArraySequence<T>* CreateFromArray(const DynamicArray<T>& other) const override {
        return new MutableArraySequence<T>(other);
    }

public:
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) {}
    MutableArraySequence(const DynamicArray<T>& other) : ArraySequence<T>(other) {}
    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>(other) {}

    MutableArraySequence<T>& operator=(const MutableArraySequence<T>& other) {
        if (this == &other)
            return *this;

        ArraySequence<T>::operator=(other);
        return *this;
    }

    T& operator[](size_t index) {
        return this->array[index];
    }

    const T& operator[](size_t index) const {
        return this->Get(index);
    }
    
    MutableArraySequence<T>* Append(const T& value) {
        this->AppendInternal(value);
        return this;
    }

    MutableArraySequence<T>* Prepend(const T& value) {
        this->PrependInternal(value);
        return this;
    }

    MutableArraySequence<T>* InsertAt(size_t index, const T& value) {
        this->InsertAtInternal(index, value);
        return this;
    }

    MutableArraySequence<T>* Remove(size_t index) {
        this->RemoveAtInternal(index);
        return this;
    }

    Sequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
};


#endif /* MUTABLE_ARRAY_SEQUENCE_HPP */