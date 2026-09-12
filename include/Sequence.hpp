#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <cstddef>

template <typename T>
class Sequence {
public:
    virtual const T& Get(size_t index) const = 0;
    virtual void Set(size_t index, const T& value) = 0;
    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual size_t GetLength() const = 0;

    int FindFirst(bool (*pred)(const T&)) const { 
        for (size_t i = 0; i < GetLength(); i++) {
            if (pred(Get(i)))
                return static_cast<int>(i);
        }
        return -1;
    }
    T Reduce(T (*func)(const T&, const T&), T initial) const {
        T result = initial;
        for (size_t i = 0; i < GetLength(); i++) {
            result = func(result, Get(i));
        }
        return result;
    }

    virtual Sequence<T>* GetSubsequence(size_t start_index, size_t end_index) const = 0;
    virtual Sequence<T>* Concat(const Sequence<T> &other) const = 0;

    virtual Sequence<T> *Clone() const = 0;

    virtual ~Sequence() = default;
};

#endif /* SEQUENCE_HPP */
