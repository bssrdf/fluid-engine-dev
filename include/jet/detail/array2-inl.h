// Copyright (c) 2016 Doyub Kim

#ifndef INCLUDE_JET_DETAIL_ARRAY2_INL_H_
#define INCLUDE_JET_DETAIL_ARRAY2_INL_H_

#include <jet/macros.h>
#include <jet/parallel.h>
#include <algorithm>
#include <vector>

namespace jet {

template <typename T>
Array<T, 2>::Array() {
}

template <typename T>
Array<T, 2>::Array(const Size2& size, const T& initVal) {
    resize(size, initVal);
}

template <typename T>
Array<T, 2>::Array(size_t width, size_t height, const T& initVal) {
    resize(width, height, initVal);
}

template <typename T>
Array<T, 2>::Array(const std::initializer_list<std::initializer_list<T>>& lst) {
    set(lst);
}

template <typename T>
Array<T, 2>::Array(const Array& other) {
    set(other);
}

template <typename T>
void Array<T, 2>::set(const T& value) {
    for (auto& v : _data) {
        v = value;
    }
}

template <typename T>
void Array<T, 2>::set(const Array& other) {
    _data.resize(other._data.size());
    std::copy(other._data.begin(), other._data.end(), _data.begin());
    _size = other._size;
}

template <typename T>
void Array<T, 2>::set(
    const std::initializer_list<std::initializer_list<T>>& lst) {
    size_t height = lst.size();
    size_t width = (height > 0) ? lst.begin()->size() : 0;
    resize(Size2(width, height));
    auto rowIter = lst.begin();
    for (size_t j = 0; j < height; ++j) {
        JET_ASSERT(width == rowIter->size());
        auto colIter = rowIter->begin();
        for (size_t i = 0; i < width; ++i) {
            (*this)(i, j) = *colIter;
            ++colIter;
        }
        ++rowIter;
    }
}

template <typename T>
void Array<T, 2>::clear() {
    _data.clear();
    _size = Size2(0, 0);
}

template <typename T>
void Array<T, 2>::resize(const Size2& size, const T& initVal) {
    Array grid;
    grid._data.resize(size.x * size.y, initVal);
    grid._size = size;
    size_t iMin = std::min(size.x, _size.x);
    size_t jMin = std::min(size.y, _size.y);
    for (size_t j = 0; j < jMin; ++j) {
        for (size_t i = 0; i < iMin; ++i) {
            grid(i, j) = at(i, j);
        }
    }

    swap(grid);
}

template <typename T>
void Array<T, 2>::resize(size_t width, size_t height, const T& initVal) {
    resize(Size2(width, height), initVal);
}

template <typename T>
T& Array<T, 2>::at(size_t i) {
    JET_ASSERT(i < _size.x*_size.y);
    return _data[i];
}

template <typename T>
const T& Array<T, 2>::at(size_t i) const {
    JET_ASSERT(i < _size.x*_size.y);
    return _data[i];
}

template <typename T>
T& Array<T, 2>::at(const Point2UI& pt) {
    return at(pt.x, pt.y);
}

template <typename T>
const T& Array<T, 2>::at(const Point2UI& pt) const {
    return at(pt.x, pt.y);
}

template <typename T>
T& Array<T, 2>::at(size_t i, size_t j) {
    JET_ASSERT(i < _size.x && j < _size.y);
    return _data[i + _size.x * j];
}

template <typename T>
const T& Array<T, 2>::at(size_t i, size_t j) const {
    JET_ASSERT(i < _size.x && j < _size.y);
    return _data[i + _size.x * j];
}

template <typename T>
Size2 Array<T, 2>::size() const {
    return _size;
}

template <typename T>
size_t Array<T, 2>::width() const {
    return _size.x;
}

template <typename T>
size_t Array<T, 2>::height() const {
    return _size.y;
}

template <typename T>
T* Array<T, 2>::data() {
    return _data.data();
}

template <typename T>
const T* const Array<T, 2>::data() const {
    return _data.data();
}

template <typename T>
typename Array<T, 2>::ContainerType::iterator Array<T, 2>::begin() {
    return _data.begin();
}

template <typename T>
typename Array<T, 2>::ContainerType::const_iterator Array<T, 2>::begin() const {
    return _data.begin();
}

template <typename T>
typename Array<T, 2>::ContainerType::iterator Array<T, 2>::end() {
    return _data.end();
}

template <typename T>
typename Array<T, 2>::ContainerType::const_iterator Array<T, 2>::end() const {
    return _data.end();
}

template <typename T>
ArrayAccessor2<T> Array<T, 2>::accessor() {
    return ArrayAccessor2<T>(size(), data());
}

template <typename T>
ConstArrayAccessor2<T> Array<T, 2>::constAccessor() const {
    return ConstArrayAccessor2<T>(size(), data());
}

template <typename T>
void Array<T, 2>::swap(Array& other) {
    std::swap(other._data, _data);
    std::swap(other._size, _size);
}

template <typename T>
template <typename Callback>
void Array<T, 2>::forEach(Callback func) {
    for (size_t j = 0; j < _size.y; ++j) {
        for (size_t i = 0; i < _size.x; ++i) {
            func(at(i, j));
        }
    }
}

template <typename T>
template <typename Callback>
void Array<T, 2>::forEachIndex(Callback func) const {
    for (size_t j = 0; j < _size.y; ++j) {
        for (size_t i = 0; i < _size.x; ++i) {
            func(i, j);
        }
    }
}

template <typename T>
template <typename Callback>
void Array<T, 2>::parallelForEach(Callback func) {
    parallelFor(kZeroSize, _size.x, kZeroSize, _size.y,
        [&](size_t i, size_t j) {
            func(at(i, j));
        });
}

template <typename T>
template <typename Callback>
void Array<T, 2>::parallelForEachIndex(Callback func) const {
    parallelFor(kZeroSize, _size.x, kZeroSize, _size.y, func);
}

template <typename T>
void Array<T, 2>::serialize(std::ostream* strm) const {
    uint64_t size64[2] = { _size.x, _size.y };

    const char* sizeAsBytes = reinterpret_cast<const char*>(&size64);
    strm->write(sizeAsBytes, 2 * sizeof(uint64_t));

    if (_size.x * _size.y > 0) {
        const char* body = reinterpret_cast<const char*>(data());
        strm->write(body, sizeof(T) * _size.x * _size.y);
    }
}

template <typename T>
void Array<T, 2>::deserialize(std::istream* strm) {
    uint64_t size64[2];
    char* sizeAsBytes = reinterpret_cast<char*>(&size64);
    strm->read(sizeAsBytes, 2 * sizeof(uint64_t));

    Size2 newSize(
        static_cast<size_t>(size64[0]),
        static_cast<size_t>(size64[1]));
    resize(newSize);
    if (newSize.x * newSize.y > 0) {
        char* body = reinterpret_cast<char*>(data());
        strm->read(body, sizeof(T) * newSize.x * newSize.y);
    }
}

template <typename T>
T& Array<T, 2>::operator[](size_t i) {
    return _data[i];
}

template <typename T>
const T& Array<T, 2>::operator[](size_t i) const {
    return _data[i];
}

template <typename T>
T& Array<T, 2>::operator()(size_t i, size_t j) {
    JET_ASSERT(i < _size.x && j < _size.y);
    return _data[i + _size.x * j];
}

template <typename T>
const T& Array<T, 2>::operator()(size_t i, size_t j) const {
    JET_ASSERT(i < _size.x && j < _size.y);
    return _data[i + _size.x * j];
}

template <typename T>
T& Array<T, 2>::operator()(const Point2UI &pt) {
    JET_ASSERT(pt.x < _size.x && pt.y < _size.y);
    return _data[pt.x + _size.x * pt.y];
}

template <typename T>
const T& Array<T, 2>::operator()(const Point2UI &pt) const {
    JET_ASSERT(pt.x < _size.x && pt.y < _size.y);
    return _data[pt.x + _size.x * pt.y];
}

template <typename T>
Array<T, 2>& Array<T, 2>::operator=(const T& value) {
    set(value);
    return *this;
}

template <typename T>
Array<T, 2>& Array<T, 2>::operator=(const Array& other) {
    set(other);
    return *this;
}

template <typename T>
Array<T, 2>& Array<T, 2>::operator=(
    const std::initializer_list<std::initializer_list<T>>& lst) {
    set(lst);
    return *this;
}

template <typename T>
Array<T, 2>::operator ArrayAccessor2<T>() {
    return accessor();
}

template <typename T>
Array<T, 2>::operator ConstArrayAccessor2<T>() const {
    return constAccessor();
}

}  // namespace jet

#endif  // INCLUDE_JET_DETAIL_ARRAY2_INL_H_