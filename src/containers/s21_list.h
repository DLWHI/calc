#ifndef SRC_CONTAINERS_S21_LIST_H_
#define SRC_CONTAINERS_S21_LIST_H_

#include <cstddef>
#include <list>
#include <ostream>
#include <stdexcept>

namespace s21 {
template <typename T>

class list {
  // Forward declaration:
  struct Node;

 public:
  // Forward declaration:
  template <typename TI>
  class ListIterator;

  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef ListIterator<Node> iterator;
  typedef ListIterator<const Node> const_iterator;
  typedef size_t size_type;

  // Default constructor
  list() : head_{value_type(), &head_, &head_}, counter_(0){};

  /*  Parameterized constructor
  in stl list is circular */
  list(size_type n) : list() {
    Node* ptr = &head_;
    for (size_type i = 0; i < n; ++i) {
      Node* some = new Node{0, ptr, &head_};
      ptr->next = some;
      head_.prev = some;
      ptr = ptr->next;
    }
    counter_ = n;
  }

  //  Initializer list constructor, creates a list initialized using
  //  std::initializer_list<T>
  list(std::initializer_list<value_type> const& items) : list() {
    Node* ptr = &head_;
    // items - initializer list
    //  we create i as an iterator to work with initializer list
    for (auto i = items.begin(); i != items.end(); ++i) {
      Node* some = new Node{*i, ptr, &head_};
      head_.prev = some;
      ptr->next = some;
      ptr = ptr->next;
    }
    counter_ = items.size();
  }

  //  Copy constructor:
  list(const list& l) : list() {
    const Node* ptrToL = l.head_.next;
    Node* ptrToCopy = &head_;
    for (size_type i = 0; i < l.counter_; ++i) {
      Node* some = new Node{ptrToL->data, ptrToCopy, &head_};
      ptrToCopy->next = some;
      head_.prev = some;
      ptrToCopy = ptrToCopy->next;
      ptrToL = ptrToL->next;
    }
    counter_ = l.counter_;
  }

  //  Move constructor (no copying for anything but
  //  primitive types and rvalue is undefined but consistent)
  list(list&& l) : list() {
    //  Using 'std::move' to make other a rvalue and make
    //  compiler to use move not copy
    *this = std::move(l);
  }

  //  Assignment operator overload for moving an object
  list& operator=(list&& l) {
    swap(l);
    return *this;
  }

  //  Assignment operator overload for copying an object
  list& operator=(const list& l) {
    list<T> cpy(l);
    swap(cpy);
    return *this;
  }

  //  Destructor
  ~list() {
    Node* ptr = end().ptr_->prev;
    for (size_t i = 0; i < counter_; ++i) {
      Node* tmp = ptr->prev;
      delete ptr;
      ptr = tmp;
    }
  }

  //  Element access:---------------------------------------------------------
  //  Returns a read-only reference to the data from the first node of the list
  const_reference front() const { return *begin(); }

  //  Returns a read-only reference to the data from the last node of the list
  const_reference back() const { return *(end() - 1); }

  //  List Iterators: ---------------------------------------------------------
  iterator begin() { return iterator(&head_) + 1; }

  iterator end() { return iterator(&head_); }

  const_iterator begin() const { return const_iterator(&head_) + 1; }

  const_iterator end() const { return const_iterator(&head_); }

  //  List Capacity:  ---------------------------------------------------------
  bool empty() const {
    if (this->counter_ == 0) {
      return 1;
    }
    return 0;
  }

  //  Returns number of elements in the list
  size_type size() { return counter_; }

  //  Returns max number of elements
  size_type max_size() const { return size_type(-1); }

  //  List Modifiers  ---------------------------------------------------------
  void clear() {
    while (counter_) {
      pop_back();
    }
  }

  iterator insert(iterator pos, const_reference value) {
    Node* some = new Node{value, pos.ptr_->prev, pos.ptr_};
    pos.ptr_->prev->next = some;
    pos.ptr_->prev = some;
    counter_ += 1;
    return iterator(some);
  }

  //  Deletes an element at pos
  void erase(iterator pos) {
    Node* nextNode = (pos + 1).ptr_;
    Node* prevNode = (pos - 1).ptr_;
    prevNode->next = nextNode;
    nextNode->prev = prevNode;
    delete pos.ptr_;
    counter_ -= 1;
  }

  void push_back(const_reference value) {
    //  insert puts a node at a position before the one iterator is pointing to
    insert(end(), value);
  }

  void pop_back() { erase(end() - 1); }

  void push_front(const_reference value) { insert(begin(), value); }

  void pop_front() { erase(begin()); }

  void swap(list& other) {
    std::swap(other.head_.prev, head_.prev);
    head_.prev->next = &head_;
    other.head_.prev->next = &other.head_;

    std::swap(other.head_.next, head_.next);
    head_.next->prev = &head_;
    other.head_.next->prev = &other.head_;

    std::swap(counter_, other.counter_);
  }

  //  Merges x into the list by transferring all of its elements at their
  //  respective ordered positions into the container (both containers shall
  //  already be ordered). This effectively removes all the elements in other
  //  (which becomes empty), and inserts them into their ordered position within
  //  container (which expands in size by the number of elements transferred).
  //  The operation is performed without constructing nor destroying any
  //  elements
  void merge(list& other) {
    iterator iter = begin();
    // Iterator iter_other = other.begin();
    while (iter != end() && other.counter_) {
      if (*iter > *(other.begin())) {
        // Head of other must point to the next after iter_other and vice versa
        Node* inserted = other.begin().ptr_;
        Node* newBegin = (other.begin() + 1).ptr_;
        other.head_.next = newBegin;
        newBegin->prev = &other.head_;
        //  Node immediately before iter now needs to point to the inserting
        //  Element and vice versa
        Node* prevNode = (iter - 1).ptr_;
        prevNode->next = inserted;
        inserted->prev = prevNode;
        //  Node at iter now needs to point to the inserting element and vice
        //  versa
        inserted->next = iter.ptr_;
        iter.ptr_->prev = inserted;
        other.counter_ -= 1;
        counter_ += 1;
      } else {
        ++iter;
      }
    }
    if (!other.empty()) {
      splice(end(), other);
    }
  }

  //  No elements are copied or moved, only the internal
  //  pointers of the list nodes are re-pointed. Transfers all
  //  elements from other into *this. The elements are inserted
  //  before the element pointed to by pos. The container other
  //  becomes empty after the operation.
  void splice(const_iterator pos, list& other) {
    Node* nextNode = const_cast<Node*>((pos).ptr_);
    Node* prevNode = const_cast<Node*>((pos - 1).ptr_);
    prevNode->next = other.begin().ptr_;
    other.begin().ptr_->prev = prevNode;
    other.end().ptr_->prev->next = nextNode;
    nextNode->prev = other.end().ptr_->prev;
    other.head_.prev = &other.head_;
    other.head_.next = &other.head_;
    counter_ += other.counter_;
    other.counter_ = 0;
  }

  void reverse() {
    Node* ptr = head_.next;
    for (size_t i = 0; i < counter_; ++i) {
      insert(ptr, back());
      pop_back();
    }
  }

  void unique() {
    iterator first = begin();
    iterator second = first + 1;
    while (second != end()) {
      if (*second == *first) {
        erase(second);
        second = first;
      } else {
        ++first;
      }
      ++second;
    }
  }

  void sort() {
    if (counter_ < 2) {
      return;
    }
    list a;
    list b;
    FrontBackSplit(*this, a, b);
    a.sort();
    b.sort();
    a.merge(b);
    swap(a);
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args) {
    list other{args...};
    iterator final_pos(const_cast<Node*>(pos.ptr_->prev));
    splice(pos, other);
    return final_pos + 1;
  }

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    insert_many(end(), args...);
  }

  template <typename... Args>
  void insert_many_front(Args&&... args) {
    insert_many(begin(), args...);
  }

  int IsEqual(const list& other) {
    if (counter_ != other.counter_) {
      return 0;
    }
    if (counter_ == 0) {
      return 1;
    }
    iterator first = begin();
    iterator second = other.head_.next;
    while (first != end()) {
      if (*first != *second) {
        return 0;
      }
      ++first;
      ++second;
    }
    return 1;
  }

  friend std::ostream& operator<<(std::ostream& os, list some) {
    Node* ptr = some.head_.next;
    for (size_t i = 0; i < some.counter_; ++i) {
      os << ptr->data << ' ';
      ptr = ptr->next;
    }
    return os;
  }

  template <typename TI>
  class ListIterator {
    /* A friend class can access private and protected members
    of other classes in which it is declared as a friend */

    friend list;

   public:
    //  Default constructor:
    ListIterator() : ptr_(nullptr) {}
    //  Parametrised constructor:
    ListIterator(TI* data) noexcept : ptr_(data){};

    //  Operators overload:
    ListIterator& operator++() noexcept {
      ptr_ = ptr_->next;
      return *this;
    }
    ListIterator& operator--() noexcept {
      ptr_ = ptr_->prev;
      return *this;
    }

    bool operator==(const ListIterator& other) const { return ptr_ == other.ptr_; }

    bool operator!=(const ListIterator& other) const { return ptr_ != other.ptr_; }

    //  - int overload used in insert:
    ListIterator operator-(int k) {
      ListIterator tmp(ptr_);  //  parametrised constructor
      for (int i = 0; i < k; ++i) {
        tmp.ptr_ = tmp.ptr_->prev;
      }
      return tmp;
    }

    ListIterator operator+(int k) {
      ListIterator tmp(ptr_);  //  parametrised constructor
      for (int i = 0; i < k; ++i) {
        tmp.ptr_ = tmp.ptr_->next;
      }
      return tmp;
    }

    //  implicit conversion
    operator ListIterator<const TI>() const {
      return ListIterator<const TI>(ptr_);
    }
    
    const T& operator*() const { return ptr_->data; }

   private:
    TI* ptr_;
  };

 private:
  struct Node {
    T data;
    Node* prev;
    Node* next;
  };
  //  This function splits a list into two halves and used in sort:
  void FrontBackSplit(list& initial, list& a, list& b) {
    iterator middle = initial.begin();
    size_t limit = (initial.counter_ % 2 > 0) ? initial.counter_ / 2 + 1
                                              : initial.counter_ / 2;
    size_t i = 0;
    for (; i < limit; ++i) {
      ++middle;
    }
    iterator copy = middle - 1;
    // To the head of b list we connect the second half of the initial list
    b.counter_ = initial.counter_ - limit;
    b.head_.next = middle.ptr_;
    middle.ptr_->prev = &b.head_;
    b.head_.prev = initial.head_.prev;
    initial.head_.prev->next = &b.head_;
    // To the head of a list we connect the first half of the initial list
    a.head_.next = initial.begin().ptr_;
    a.head_.prev = copy.ptr_;
    copy.ptr_->next = &a.head_;
    initial.begin().ptr_->prev = &a.head_;
    a.counter_ = limit;
    //  We take care of the head of the initial list and fix the counter of it
    initial.head_.prev = &initial.head_;
    initial.head_.next = &initial.head_;
    initial.counter_ = 0;
  }

  Node head_;
  size_t counter_;
};

}  // namespace s21

#endif  // SRC_CONTAINERS_S21_LIST_H_
