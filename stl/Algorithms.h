#pragma once
#include "Container.h"
#include "Iterator.h"
#include <iostream>

template <class Iterator>
concept Iterable = requires(Iterator first, Iterator second) {
  { first != second } -> std::convertible_to<bool>;
  { first == second } -> std::convertible_to<bool>;
  { *first } -> std::same_as<std::iter_reference_t<Iterator>>;
  *first = *second;
  first = second;
  { ++first } -> std::same_as<Iterator &>;
  { --first } -> std::same_as<Iterator &>;
  { first++ } -> std::same_as<Iterator>;
  { first-- } -> std::same_as<Iterator>;
};

template <class Iterator, class Predicate>
concept ConceptPredicate =
    requires(Iterator first, Iterator second, Predicate predicate) {
      { predicate(*first, *second) } -> std::convertible_to<bool>;
    };

template <class Iterator, class Function>
concept ConceptLambda = requires(Function lambda, Iterator it) { lambda(*it); };

template <class Iterator, class Function>
concept Condition = requires(Iterator it, Function expression) {
  { expression(*it) } -> std::convertible_to<bool>;
};

template <class Iterator, class Function>
  requires(Iterable<Iterator>, Condition<Iterator, Function>)
Iterator findIf(Iterator beg, Iterator end, Function f) {
  for (; beg != end; ++beg) {
    if (f(*beg)) {
      return beg;
    }
  }

  return end;
}

template <class Iterator, class Predicate>
  requires Iterable<Iterator> && ConceptPredicate<Iterator, Predicate>
Iterator maxElement(Iterator beg, Iterator end, Predicate comp) {
  Iterator res = beg;
  ++beg;
  for (; beg != end; ++beg) {
    if (comp(*res, *beg)) {
      res = beg;
    }
  }

  return res;
}

template <class Iterator, class Predicate>
  requires Iterable<Iterator> && ConceptPredicate<Iterator, Predicate>
Iterator minElement(Iterator beg, Iterator end, Predicate comp) {
  Iterator res = beg;
  ++beg;
  for (; beg != end; ++beg) {
    if (comp(*beg, *res)) {
      res = beg;
    }
  }

  return res;
}

template <class Iterator, class Predicate>
  requires Iterable<Iterator> && ConceptPredicate<Iterator, Predicate>
void Sort(Iterator beg, Iterator end, Predicate comp) {
  for (; beg != end; beg++) {
    std::iter_swap(beg, minElement(beg, end, comp));
  }
}

template <class Iterator, class Function>
  requires Iterable<Iterator> && Condition<Iterator, Function>
void copyIf(Iterator sourceBeg, Iterator sourceEnd, Iterator destBeg,
            Function f) {
  for (; sourceBeg != sourceEnd; ++sourceBeg) {
    if (f(*sourceBeg)) {
      *destBeg = *sourceBeg;
      ++destBeg;
    }
  }
}

template <class Iterator, class Operation>
  requires Iterable<Iterator> && ConceptLambda<Iterator, Operation>
void forEach(Iterator beg, Iterator end, Operation op) {
  for (; beg != end; ++beg) {
    op(*beg);
  }
}

template <class _BidirectionalIter>
void sort(_BidirectionalIter beg, _BidirectionalIter end) {
  if (end <= beg) {
    return;
  }
  _BidirectionalIter pivot = beg, middle = beg + 1;
  for (_BidirectionalIter i = middle; i < end; ++i) {
    if (*i < *pivot) {
      std::iter_swap(i, middle);
      ++middle;
    }
  }
  std::iter_swap(beg, middle - 1);
  sort(beg, middle - 1);
  sort(middle, end);
}

template <class _InputIter, class _OutputIter, class _UnaryPred>
_OutputIter copyIf(_InputIter source_beg, _InputIter source_end,
                   _OutputIter dest_beg, _UnaryPred pred) {
  while (source_beg != source_end) {
    if (pred(*source_beg)) {
      *dest_beg = *source_beg;
      ++dest_beg;
    }
    ++source_beg;
  }
  return dest_beg;
}
