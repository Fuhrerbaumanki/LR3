#pragma once

#include <stdexcept>
template <typename Container> class VectorIterator {
private:
  typename Container::value_type *ptr; // Указатель на текущий элемент вектора

public:
  // Конструктор итератора
  explicit VectorIterator(Container &vec, size_t index = 0) {
    if (index >= vec.size()) {
      throw std::out_of_range("Invalid index");
    }
    ptr = &vec[index];
  }

  // Копирующий конструктор
  VectorIterator(const VectorIterator &other) : ptr(other.ptr) {}

  // Деструктор
  ~VectorIterator() = default;
};
