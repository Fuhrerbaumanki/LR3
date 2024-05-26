#pragma once
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
  UnorderedMapIterator(const UnorderedMapIterator &p_other)
      : m_ptr(p_other.m_ptr) {}

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

template <typename _KeyType, typename _DataType>
std::ostream &my_ostream(std::ostream &os,
                         const UnorderedMapIterator<_KeyType, _DataType> &it) {
  if (it.GetPtr() == nullptr) {
    os << "Iterator points to nullptr";
  } else {
    const auto &pair = *it;
    os << "|  Id: " << std::setw(5) << pair.first
       << "  |  Name: " << std::setw(10) << pair.second.m_name
       << "  |  Manufacturer: " << std::setw(10) << pair.second.m_manufacturer
       << "  |  Warehouse address: " << std::setw(30)
       << pair.second.m_warehouse_address << "  |  Weight: " << std::setw(5)
       << pair.second.m_weight << "  |";
  }
  return os;
}
