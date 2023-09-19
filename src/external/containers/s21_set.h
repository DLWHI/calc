#ifndef SRC_CONTAINERS_S21_SET_H_
#define SRC_CONTAINERS_S21_SET_H_
#include "s21_red_black_tree.h"
#include "vector.h"

namespace s21 {
template <typename T>
class set {
 public:
  typedef RedBlackTree<T, T> rbtree;
  typedef T key_type;
  typedef T value_type;
  typedef T &reference;
  typedef const T &const_reference;
  typedef typename rbtree::key_iterator iterator;
  typedef typename rbtree::const_key_iterator const_iterator;
  typedef size_t size_type;

  set() = default;

  set(std::initializer_list<value_type> const &items) {
    for (auto i = items.begin(); i != items.end(); ++i) {
      if (!tree_.root_->SearchNode(*i)) tree_.insert(*i, *i);
    }
  };

  set(const set &s) : tree_(s.tree_){};

  set(set &&s) : tree_(std::move(s.tree_)){};

  set &operator=(const set &s) {
    set cpy(s);
    *this = std::move(cpy);
    return *this;
  }

  set &operator=(set &&s) {
    tree_ = std::move(s.tree_);
    return *this;
  }
  ~set() = default;

  iterator begin() { return tree_.kbegin(); }

  iterator end() { return tree_.kend(); }

  const_iterator begin() const { return tree_.kbegin(); }

  const_iterator end() const { return tree_.kend(); }

  bool empty() const { return !tree_.count_; }

  size_type size() const { return tree_.count_; }

  size_type max_size() const { return size_type(-1); }

  void clear() {
    for (auto it = begin(), et = end(); it != et;) {
      iterator cpy(it);
      ++it;
      tree_.remove(*cpy);
    }
  }

  std::pair<iterator, bool> insert(const value_type &value) {
    bool exist = tree_.root_->SearchNode(value);
    if (!exist) {
      tree_.insert(value, value);
    }
    iterator pos(tree_.root_->SearchNode(value));
    return std::make_pair(pos, !exist);
  }

  void erase(iterator pos) {
    if (pos != end()) {
      tree_.remove(*pos);
    }
  }

  void swap(set &other) { tree_ = std::move(other.tree_); }

  void merge(set &other) {
    for (auto it = other.begin(), et = other.end(); it != et; ++it) {
      insert(*it);
    }
    other.clear();
  }

  iterator find(const value_type &key) {
    auto node = tree_.root_->SearchNode(key);
    if (node)
      return iterator(node);
    else
      return ++iterator(tree_.root_->Max());
  }

  bool contains(const value_type &key) const {
    return (tree_.root_->SearchNode(key)) ? true : false;
  }

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    std::initializer_list elements{args...};
    vector<std::pair<iterator, bool>> insert_result(elements.size());
    for (auto elem : elements) {
      insert_result.push_back(insert(elem));
    }
    return insert_result;
  }

  void print() { tree_.print(); }

 private:
  rbtree tree_;
};
}  // namespace s21
#endif  // SRC_CONTAINERS_S21_SET_H_
