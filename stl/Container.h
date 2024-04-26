#pragma once
#include <algorithm>
#include <initializer_list>
#ifndef CONTAINER_H
#define CONTAINER_H

#include <iostream>

template <typename _DataType> class Vector {
private:
  _DataType *data; // Указатель на массив данных
  size_t size;     // Размер вектора
  size_t _capacity;

public:
  // Конструкторы
  // Конструктор по умолчанию
  Vector() : data(nullptr), size(0) {}

  // Конструктор с заданным размером
  Vector(size_t _size) : size(_size) { data = new _DataType[size](); }

  // Конструктор с заданным размером и значением для инициализации элементов
  Vector(size_t _size, const _DataType &value) : size(_size) {
    data = new _DataType[size];
    for (size_t i = 0; i < size; ++i) {
      data[i] = value;
    }
  }

  // Конструктор с полуинтервалом [beg, end) заданным итераторами
  template <typename Iterator>
  Vector(Iterator beg, Iterator end) : size(std::distance(beg, end)) {
    data = new _DataType[size];
    std::copy(beg, end, data);
  }

  // Конструктор с использованием списка инициализации
  Vector(std::initializer_list<_DataType> initList) : size(initList.size()) {
    data = new _DataType[size];
    std::copy(initList.begin(), initList.end(), data);
  }

  // Копирующий конструктор
  Vector(const Vector<_DataType> &other) : size(other.size) {
    data = new _DataType[size];
    std::copy(other.data, other.data + size, data);
  }

  // Конструктор переноса
  Vector(Vector<_DataType> &&other) noexcept
      : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
  }

  // Метод для динамического изменения размера вектора
  void resizeCapacity() {
    size_t newCapacity =
        (_capacity == 0) ? 1 : 2 * _capacity; // Удваиваем размер
    _DataType *newData = new _DataType[newCapacity];
    std::copy(data, data + size, newData);
    delete[] data;
    data = newData;
    _capacity = newCapacity;
  }

  // Функции-операции
  // Оператор присваивания с копированием
  Vector<_DataType> &operator=(const Vector<_DataType> &other) {
    if (this != &other) {
      delete[] data;
      size = other.size;
      data = new _DataType[size];
      std::copy(other.data, other.data + size, data);
    }
    return *this;
  }

  // Операция присваивания с переносом
  Vector<_DataType> &operator=(Vector<_DataType> &&other) noexcept {
    if (this != &other) {
      delete[] data;
      data = other.data;
      size = other.size;
      other.data = nullptr;
      other.size = 0;
    }
    return *this;
  }

  // Оператор индексирования для чтения
  _DataType &operator[](size_t index) { return data[index]; }

  // Оператор индексирования для записи
  const _DataType &operator[](size_t index) const { return data[index]; }

  // Методы
  // Возвращает элемент с индексом idx
  _DataType &at(size_t idx) {
    if (idx >= size) {
      throw std::out_of_range("Выход за пределы массива");
    }
    return data[idx];
  }

  // Метод begin() – возвращает итератор на начало вектора
  _DataType *begin() { return data; }

  // Метод end() – возвращает итератор на элемент, следующий за последним
  _DataType *end() { return data + size; }

  // Добавляет копию аргумента elem в конец вектора (обычная версия)
  void pushBack(const _DataType &elem) {
    if (size == _capacity) {
      resizeCapacity();
    }
    data[size++] = elem;
  }

  // Добавляет копию аргумента elem в конец вектора (версия с переносом)
  void pushBack(_DataType &&elem) {
    _DataType *newData = new _DataType[size + 1];
    std::copy(data, data + size, newData);
    newData[size] = std::move(elem);
    delete[] data;
    data = newData;
    ++size;
  }

  // Метод popBack() – удаляет последний элемент
  void popBack() {
    if (size > 0) {
      --size;
      _DataType *newData = new _DataType[size];
      std::copy(data, data + size, newData);
      delete[] data;
      data = newData;
    }
  }

  // Метод insert(pos, elem) – вставляет копию элемента elem перед позицией
  // итератора pos (обычная версия)
  iterator insert(iterator pos, const _DataType &elem) {
    size_t index = pos - data;
    if (index > size) {
      throw std::out_of_range("Недопустимая позиция итератора");
    }
    if (size == _capacity) {
      resizeCapacity();
    }
    _DataType *newData = new _DataType[size + 1];
    std::copy(data, data + index, newData);
    newData[index] = elem;
    std::copy(data + index, data + size, newData + index + 1);
    delete[] data;
    data = newData;
    ++size;
    return data + index;
  }

  // Метод insert(pos, elem) – вставляет копию элемента elem перед позицией
  // итератора pos (версия с переносом)
  iterator insert(iterator pos, _DataType &&elem) {
    size_t index = pos - data;
    if (index > size) {
      throw std::out_of_range("Недопустимая позиция итератора");
    }
    _DataType *newData = new _DataType[size + 1];
    std::copy(data, data + index, newData);
    newData[index] = std::move(elem);
    std::copy(data + index, data + size, newData + index + 1);
    delete[] data;
    data = newData;
    ++size;
    return data + index;
  }

  // Вставляет копии всех элементов интервала
  iterator insert(iterator pos, Iterator beg, Iterator end) {
    size_t index = pos - data;
    size_t count = std::distance(beg, end);
    if (count == 0) {
      return pos;
    }
    _DataType *newData = new _DataType[size + count];
    std::copy(data, data + index, newData);
    std::copy(beg, end, newData + index);
    std::copy(data + index, data + size, newData + index + count);
    delete[] data;
    data = newData;
    size += count;
    return data + index;
  }

  // Удаляет элемент в позиции итератора pos и возвращает позицию след эл-та
  iterator erase(iterator pos) {
    size_t index = pos - data;
    if (index >= size) {
      throw std::out_of_range("Недопустимая позиция итератора");
    }
    std::copy(data + index + 1, data + size, data + index);
    --size;
    return data + index;
  }

  // Метод reserve(num) – увеличивает емкость (объем зарезервированной памяти)
  void reserve(size_t num) {
    if (num <= capacity()) {
      return; // Новая емкость не меньше текущей
    }

    // Создаем новый буфер с увеличенной емкостью
    _DataType *newData = new _DataType[num];

    // Копируем существующие элементы в новый буфер
    std::copy(data, data + size, newData);

    // Освобождаем память из старого буфера
    delete[] data;

    // Обновляем указатель на данные и емкость вектора
    data = newData;
  }

  // Метод resize(num) – изменяет количество элементов до num
  void resize(size_t num) {
    if (num > size) {
      // Увеличиваем размер вектора
      reserve(num); // Убедимся, что есть достаточно места в памяти
      for (size_t i = size; i < num; ++i) {
        data[i] =
            _DataType(); // Создаем новые элементы конструктором по умолчанию
      }
    } else {
      // Уменьшаем размер вектора
      size = num;
    }
  }

  // Проверяет контейнер на пустоту
  bool empty() const { return size == 0; }

  // Метод size() – возвращает размер вектора
  size_t size_vecor() const { return size; }

  // Возвращает количество элементов, максимально возможное
  size_t capacity() const { return _capacity; }

  // Метод clear() – удаляет все элементы вектора
  void clear() { size = 0; }

  // Деструктор
  ~Vector() { delete[] data; }
};

#endif // CONTAINER_H
