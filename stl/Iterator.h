#pragma once
#include "BadIteratorException.h"
#include "Container.h"
#include "RemoveException.h"

template <class _T> class List {
private:
  struct Node {
    _T value;
    Node *next;
    Node(const _T &val, Node *n = nullptr) : value(val), next(n) {}
  };

  Node *head;

public:
  class Iterator {
  private:
    Node *node;

  public:
    Iterator(Node *n) : node(n) {}

    Iterator &operator++() {
      if (node) {
        node = node->next;
      }
      return *this;
    }

    Iterator operator++(int) {
      Iterator it = *this;
      ++(*this);
      return it;
    }

    Iterator operator->() const {
      if (node == nullptr) {
        throw BadIteratorException();
      } else {
        return &(node->value);
      }
    }

    bool operator==(const Iterator &other) const { return node == other.node; }

    bool operator!=(const Iterator &other) const { return !(*this == other); }

    _T &operator*() const { return node->value; }
  };

  List() : head(nullptr) {}
  ~List() { clear(); }

  void push_front(const _T &value) { head = new Node(value, head); }

  void pop_front() {
    if (head) {
      Node *temp = head;
      head = head->next;
      delete temp;
    }
  }

  bool empty() const { return head == nullptr; }

  void clear() {
    while (!empty()) {
      pop_front();
    }
  }

  _T front() { return head->value; }

  Iterator begin() const { return Iterator(head); }

  Iterator end() const { return Iterator(nullptr); }

  Iterator find(const _T &value) const {
    Node *current = head;
    while (current != nullptr) {
      if (current->value == value) {
        return Iterator(current);
      }
      current = current->next;
    }
    return end();
  }

  Iterator erase(Iterator pos) {
    if (pos != end()) {
      Node *toRemove = pos.node;
      Iterator next(toRemove->next);

      if (toRemove == head) {
        head = head->next;
      } else {
        Node *current = head;
        while (current->next != toRemove) {
          current = current->next;
        }
        current->next = toRemove->next;
      }

      delete toRemove;
      return next;
    }
    return pos;
  }
};
