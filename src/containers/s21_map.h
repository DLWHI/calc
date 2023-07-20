#ifndef SRC_CONTAINERS_S21_MAP_H_
#define SRC_CONTAINERS_S21_MAP_H_
#include "s21_red_black_tree.h"

namespace s21 {

template <typename K, typename V>
class map {
 public:
  typedef RedBlackTree<K, V> rbtree;
  typedef K key_type;
  typedef V mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef typename rbtree::iterator iterator;
  typedef typename rbtree::const_iterator const_iterator;
  typedef size_t size_type;

  map() = default;

  map(std::initializer_list<value_type> const &items) {
    for (auto i = items.begin(); i != items.end(); ++i) {
      if (!tree_.root_->SearchNode(i->first)) tree_.insert(i->first, i->second);
    }
  };

  map(const map &s) : tree_(s.tree_){};

  map(map &&s) : tree_(std::move(s.tree_)){};

  map &operator=(const map &s) {
    map cpy(s);
    *this = std::move(cpy);
    return *this;
  }

  map &operator=(map &&s) {
    tree_ = std::move(s.tree_);
    return *this;
  }

  ~map() = default;

  iterator begin() { return tree_.begin(); }
  iterator end() { return tree_.end(); }

  const_iterator begin() const { return tree_.begin(); }
  const_iterator end() const { return tree_.end(); }

  bool empty() const { return !tree_.count_; }
  size_type size() const { return tree_.count_; }
  size_type max_size() const { return size_type(-1); }

  void clear() {
    for (auto it = begin(), et = end(); it != et;) {
      iterator cpy(it);
      ++it;
      tree_.remove(cpy->key);
    }
  }

  std::pair<iterator, bool> insert(const value_type &value) {
    bool exist = tree_.root_->SearchNode(value.first);
    if (!exist) {
      tree_.insert(value.first, value.second);
    }
    iterator pos(tree_.root_->SearchNode(value.first));
    return std::make_pair(pos, !exist);
  }

  std::pair<iterator, bool> insert(const key_type &key,
                                   const mapped_type &obj) {
    bool exist = tree_.root_->SearchNode(key);
    if (!exist) {
      tree_.insert(key, obj);
    }
    iterator pos(tree_.root_->SearchNode(key));
    return std::make_pair(pos, !exist);
  }

  std::pair<iterator, bool> insert_or_assign(const key_type &key,
                                             const mapped_type &obj) {
    bool exist = tree_.root_->SearchNode(key);
    if (!exist) {
      tree_.insert(key, obj);
    } else {
      tree_.root_->SearchNode(key)->value = obj;
    }
    iterator pos(tree_.root_->SearchNode(key));
    return std::make_pair(pos, !exist);
  }

  void erase(iterator pos) {
    if (pos != end()) {
      tree_.remove(pos->key);
    }
  }

  void swap(map &other) { tree_ = std::move(other.tree_); }

  void merge(map &other) {
    for (auto it = other.begin(), et = other.end(); it != et; ++it) {
      insert(it->key, it->value);
    }
    other.clear();
  }

  bool contains(const key_type &key) const {
    return (tree_.root_->SearchNode(key)) ? true : false;
  }

  iterator find(const key_type &key) const {
    return iterator(tree_.root_->SearchNode(key));
  }

  mapped_type &at(const key_type &key) {
    if (!tree_.root_->SearchNode(key))
      throw std::out_of_range("No such element in map");
    return tree_.root_->SearchNode(key)->value;
  }

  mapped_type &operator[](const key_type &key) {
    if (!tree_.root_->SearchNode(key)) tree_.insert(key, mapped_type());
    return tree_.root_->SearchNode(key)->value;
  }

 private:
  rbtree tree_;
};
}  // namespace s21

#endif  // SRC_CONTAINERS_S21_MAP_H_
