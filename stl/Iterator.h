#pragma once
#include "BadIteratorException.h"
#include <iomanip>
#include <iostream>
#include <utility>

template <typename _KeyType, typename _DataType> class Node;

template <typename _KeyType, typename _DataType> class UnorderedMapIterator {
  using PairType = std::pair<const _KeyType, _DataType>;
  using NodeType = Node<_KeyType, _DataType>;

private:
  NodeType *m_ptr;

public:
  UnorderedMapIterator(NodeType *p_ptr = nullptr) : m_ptr(p_ptr) {}

  UnorderedMapIterator &operator=(const UnorderedMapIterator &p_other) {
    if (this == &p_other) {
      return *this;
    }
    m_ptr = p_other.m_ptr;
    return *this;
  }

  UnorderedMapIterator &operator=(const NodeType *p_ptr) {
    m_ptr = p_ptr;
    return *this;
  }

  const PairType &operator*() const {
    if (!m_ptr) {
      throw IteratorError(
          "IteratorError: iterator does not point to an element.");
    }
    return m_ptr->m_pair;
  }

  PairType &operator*() {
    if (!m_ptr) {
      throw IteratorError(
          "IteratorError: iterator does not point to an element.");
    }
    return m_ptr->m_pair;
  }

  UnorderedMapIterator &operator++() {
    if (!m_ptr) {
      throw IteratorError(
          "IteratorError: iterator does not point to an element.");
    }
    m_ptr = m_ptr->m_next;
    return *this;
  }

  UnorderedMapIterator operator++(int) {
    if (!m_ptr) {
      throw IteratorError(
          "IteratorError: iterator does not point to an element.");
    }
    UnorderedMapIterator temp(*this);
    m_ptr = m_ptr->m_next;
    return temp;
  }

  bool operator==(const UnorderedMapIterator &p_other) const {
    return m_ptr == p_other.m_ptr;
  }

  bool operator!=(const UnorderedMapIterator &p_other) const {
    return !operator==(p_other);
  }

  PairType *operator->() {
    if (!m_ptr) {
      throw IteratorError(
          "IteratorError: iterator does not point to an element.");
    }
    return &(m_ptr->m_pair);
  }

  int GetBucketNumber() const {
    if (!m_ptr) {
      throw IteratorError(
          "IteratorError: iterator does not point to an element.");
    }
    return m_ptr->m_bucket_number;
  }

  NodeType *GetPtr() const { return m_ptr; }
};

template <class Pair, class CharT = char,
          class Traits = std::char_traits<CharT>>
class map_ostream_iterator {
private:
  std::basic_ostream<CharT, Traits> *m_stream;
  const CharT *m_delimiter;

public:
  map_ostream_iterator(std::basic_ostream<CharT, Traits> &stream,
                       const CharT *delimiter = "")
      : m_stream(&stream), m_delimiter(delimiter) {}

  template <class K, class V>
  map_ostream_iterator &operator=(const std::pair<K, V> &value) {
    *m_stream << value.first << ": " << value.second << m_delimiter;
    return *this;
  }

  map_ostream_iterator &operator*() { return *this; }

  map_ostream_iterator &operator++() { return *this; }

  map_ostream_iterator &operator++(int) { return *this; }
};
