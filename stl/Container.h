#pragma once

#include "BadIteratorException.h"
#include "Iterator.h"
#include "RemoveException.h"
#include <iostream>

template <class _KeyType, class _DataType, class _Hash> class UnorderedMap {
private:
  using Pair = std::pair<_KeyType, _DataType>;

  static constexpr size_t INITIAL_SIZE = 16;
  static constexpr float DEFAULT_LOAD_FACTOR = 0.75;

  List<Pair> *m_table;
  size_t m_capacity;
  size_t m_size;
  float maxLoadFactor;

  _Hash hashFunction;

  void rehash(size_t newCapacity) {
    List<Pair> *newTable = new List<Pair>[newCapacity]();
    for (size_t i = 0; i < m_capacity; i++) {
      List<Pair> &chain = m_table[i];
      while (!chain.empty()) {
        Pair &pair = chain.front();
        size_t index = hashFunction(pair.first) % newCapacity;
        newTable[index].push_front(pair);
        chain.pop_front();
      }
    }
    delete[] m_table;
    m_table = newTable;
    m_capacity = newCapacity;
  }

public:
  UnorderedMap() : UnorderedMap(INITIAL_SIZE) {}

  explicit UnorderedMap(size_t segmentCount)
      : m_table(new List<Pair>[segmentCount]()), m_capacity(segmentCount),
        m_size(0), maxLoadFactor(DEFAULT_LOAD_FACTOR) {}

  template <class Iterator>
  UnorderedMap(Iterator begin, Iterator end) : UnorderedMap() {
    insert(begin, end);
  }

  UnorderedMap(std::initializer_list<Pair> init) : UnorderedMap() {
    insert(init);
  }

  UnorderedMap(const UnorderedMap &other)
      : UnorderedMap(other.begin(), other.end()) {}

  UnorderedMap(UnorderedMap &&other) noexcept
      : m_table(other.m_table), m_capacity(other.m_capacity),
        m_size(other.m_size), maxLoadFactor(other.maxLoadFactor),
        hashFunction(std::move(other.hashFunction)) {
    other.m_table = nullptr;
    other.m_capacity = 0;
    other.m_size = 0;
  }

  ~UnorderedMap() { delete[] m_table; }

  UnorderedMap &operator=(const UnorderedMap &other) {
    if (this != &other) {
      UnorderedMap temp(other);
      std::swap(m_table, temp.m_table);
      std::swap(m_capacity, temp.m_capacity);
      std::swap(m_size, temp.m_size);
      std::swap(maxLoadFactor, temp.maxLoadFactor);
      std::swap(hashFunction, temp.hashFunction);
    }
    return *this;
  }

  UnorderedMap &operator=(UnorderedMap &&other) noexcept {
    if (this != &other) {
      delete[] m_table;
      m_table = other.m_table;
      m_capacity = other.m_capacity;
      m_size = other.m_size;
      maxLoadFactor = other.maxLoadFactor;
      hashFunction = std::move(other.hashFunction);

      other.m_table = nullptr;
      other.m_capacity = 0;
      other.m_size = 0;
    }
    return *this;
  }

  _DataType &operator[](const _KeyType &key) {
    Pair pair(key, _DataType());
    size_t index = hashFunction(pair.first) % m_capacity;
    List<Pair> &chain = m_table[index];
    auto it = find(chain, key);
    if (it != chain.end()) {
      return (*it).second;
    }
    chain.push_front(pair);
    m_size++;
    if (static_cast<float>(m_size) / m_capacity > maxLoadFactor) {
      rehash(m_capacity * 2);
      index = hashFunction(pair.first) % m_capacity;
      chain = m_table[index];
    }
    return chain.front().second;
  }

  float getMaxLoadFactor() const { return maxLoadFactor; };

  void setMaxLoadFactor(float value) { maxLoadFactor = value; };

  _DataType &at(const _KeyType &key) {
    size_t index = hashFunction(key) % m_capacity;
    List<Pair> &chain = m_table[index];
    // List<Pair>::Iterator it = find(chain, key);
    auto it = find(chain, key);
    if (it != chain.end()) {
      return (*it).second;
    }
    throw std::out_of_range("UnorderedMap::at() key not found");
  }

  List<Pair>::Iterator begin() const {
    for (size_t i = 0; i < m_capacity; i++) {
      if (!m_table[i].empty()) {
        return m_table[i].begin();
      }
    }
    return end();
  }

  List<Pair>::Iterator end() const { return nullptr; }

  template <class Iterator> void insert(Iterator begin, Iterator end) {
    for (Iterator it = begin; it != end; ++it) {
      insert(*it);
    }
  }

  void insert(const Pair &pair) { operator[](pair.first) = pair.second; }

  void insert(Pair &&pair) { operator[](pair.first) = std::move(pair.second); }

  void erase(const _KeyType &key) {
    size_t index = hashFunction(key) % m_capacity;
    List<Pair> &chain = m_table[index];
    ////List<Pair>::Iterator it = find(chain, key);
    auto it = find(chain, key);
    if (it != chain.end()) {
      chain.erase(it);
      m_size--;
    }
  }

  List<Pair>::Iterator erase(List<Pair>::Iterator pos) {
    size_t index = hashFunction((*pos).first) % m_capacity;
    List<Pair> &chain = m_table[index];
    auto next = chain.erase(pos);
    m_size--;
    return next;
  }

  void reserve(size_t num) {
    size_t newCapacity = num / maxLoadFactor;
    if (newCapacity > m_capacity) {
      rehash(newCapacity);
    }
  }

  size_t size() const { return m_size; }

  void clear() {
    for (size_t i = 0; i < m_capacity; i++) {
      m_table[i].clear();
    }
    m_size = 0;
  }

private:
  List<Pair>::Iterator find(List<Pair> &chain, const _KeyType &key) const {
    for (auto it = chain.begin(); it != chain.end(); ++it) {
      if ((*it).first == key) {
        return it;
      }
    }
    return chain.end();
  }
};
