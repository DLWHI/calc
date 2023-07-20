#ifndef SRC_CONTAINERS_S21_RED_BLACK_TREE_H_
#define SRC_CONTAINERS_S21_RED_BLACK_TREE_H_

#include <iostream>

namespace s21 {

enum class Color {
  kRed,
  kBlack,
};

template <typename K, typename V>
class RedBlackTree final {
 public:
  //  private:
  class Node {
   public:
    Node(K key, V value) : key(key), value(value){};
    Node(const Node &other)
        : key(other.key), value(other.value), color_(other.color_) {}
    Node(Node &&other) { *this = std::move(other); }
    Node &operator=(const Node &other) {
      Node cpy(other);
      *this = std::swap(cpy);
      return *this;
    }
    Node &operator=(Node &&other) {
      std::swap(key, other.key);
      std::swap(value, other.value);
      std::swap(parent_, other.value);
      std::swap(left_, other.left_);
      std::swap(right_, other.right_);
      std::swap(color_, other.color_);
      return *this;
    }
    ~Node() {
      delete left_;
      delete right_;
    };

    void CopyHelper(Node *copy) {
      if (copy->left_) {
        left_ = new Node(*(copy->left_));
        left_->parent_ = this;
        left_->CopyHelper(copy->left_);
      }
      if (copy->right_) {
        right_ = new Node(*(copy->right_));
        right_->parent_ = this;
        right_->CopyHelper(copy->right_);
      }
    };

    Node *Granny() {
      if (parent_) {
        return parent_->parent_;
      }
      return nullptr;
    };
    Node *Uncle() {
      Node *granny = Granny();
      if (!granny) {
        return nullptr;
      }
      if (parent_ == granny->right_) {
        return granny->left_;
      }
      return granny->right_;
    };
    Node *Brother() {
      if (!parent_) {
        return nullptr;
      } else if (!parent_->right_ || parent_->right_ == this) {
        return parent_->left_;
      }
      return parent_->right_;
    };
    Node *Max() {
      Node *maximum = this;
      while (maximum && maximum->right_) {
        maximum = maximum->right_;
      }
      return maximum;
    };
    Node *Min() {
      Node *minimum = this;
      while (minimum && minimum->left_) {
        minimum = minimum->left_;
      }
      return minimum;
    };

    void GetNodeInfo() {
      std::cout << "~~~~~~~~~~~~~~~~~~\n";
      std::cout << "Key: " << key << "; Val: " << value << std::endl;
      std::cout << "P: " << (parent_ ? parent_->key : -1)
                << "; L: " << (left_ ? left_->key : -1)
                << "; R: " << (right_ ? right_->key : -1) << std::endl;
      std::cout << "~~~~~~~~~~~~~~~~~~\n";
    }

    Node *SearchNode(K key) {
      Node *deep = this;
      while (deep != nullptr && deep->key != key) {
        if (deep->key <= key) {
          deep = deep->right_;
        } else {
          deep = deep->left_;
        }
      }
      return deep;
    };

    Node *FindFirstOf(K key) {
      Node *deep = SearchNode(key);
      while (deep != nullptr && deep->left_ && deep->left_->key == key) {
        deep = deep->left_;
      }
      return deep;
    };

    Node *Swap(Node *child, RedBlackTree *tree) {
      if (!parent_) {
        tree->root_ = child;
      } else if (parent_->left_ == this) {
        parent_->left_ = child;
      } else {
        parent_->right_ = child;
      }
      if (child) {
        Node *tmp = child->parent_;
        child->parent_ = parent_;
        if (tmp != this) {
          parent_ = tmp;
        } else {
          parent_ = child;
        }
      }
      if (left_ && left_ != child) {
        child->left_ = left_;
        left_->parent_ = child;
      }
      if (right_ && right_ != child) {
        child->right_ = right_;
        right_->parent_ = child;
      }
      left_ = right_ = nullptr;
      if (child) return child;
      return this;
    };

    void PrintHelper(std::string indent, bool last, int depth) const {
      if (depth < 10) {
        std::cout << indent;
        if (last) {
          std::cout << "R----";
          indent += "     ";
        } else {
          std::cout << "L----";
          indent += "|    ";
        }
        std::string s_color = color_ == Color::kRed ? "R" : "B";
        std::cout << "(" << key << ")"
                  << "(" << s_color << ")";
        std::cout << std::endl;
        if (left_ != nullptr) left_->PrintHelper(indent, false, depth + 1);
        if (right_ != nullptr) right_->PrintHelper(indent, true, depth + 1);
      }
    };

    Node *Bigger() const {
      Node *node = const_cast<Node *>(this);
      if (node->right_) {
        node = node->right_->Min();
      } else {
        Node *node_parent = node->parent_;
        while (node_parent && node_parent->right_ == node) {
          node = node_parent;
          node_parent = node_parent->parent_;
        }
        node = node_parent;
      }
      return node;
    }

    Node *Lower() const {
      Node *node = const_cast<Node *>(this);
      if (node->left_) {
        node = node->left_->Max();
      } else {
        Node *node_parent = node->parent_;
        while (node_parent && node_parent->left_ == node) {
          node = node_parent;
          node_parent = node_parent->parent_;
        }
        node = node_parent;
      }
      return node;
    }

    bool operator==(const Node &other) const {
      return key == other.key && value == other.value;
    }
    bool operator!=(const Node &other) const {
      return key != other.key || value != other.value;
    }

    const K key;
    V value;

   private:
    friend class RedBlackTree;

    void RotationLeft(RedBlackTree *tree) {
      Node *replace = right_;
      replace->parent_ = parent_;
      if (!replace->parent_) {
        tree->root_ = replace;
      } else if (parent_->left_ == this) {
        parent_->left_ = replace;
      } else {
        parent_->right_ = replace;
      }
      right_ = replace->left_;
      if (right_) {
        right_->parent_ = this;
      }
      parent_ = replace;
      replace->left_ = this;
    };

    void RotationRight(RedBlackTree *tree) {
      Node *replace = left_;
      replace->parent_ = parent_;
      if (!replace->parent_) {
        tree->root_ = replace;
      } else if (parent_->left_ == this) {
        parent_->left_ = replace;
      } else {
        parent_->right_ = replace;
      }
      left_ = replace->right_;
      if (left_) {
        left_->parent_ = this;
      }
      parent_ = replace;
      replace->right_ = this;
    };

    void AfterInsertFix(RedBlackTree *tree) {
      if (parent_ == NULL) {
        color_ = Color::kBlack;
        return;
      }
      if (parent_->color_ == Color::kBlack) return;
      Node *uncle = Uncle();
      if (uncle && uncle->color_ == Color::kRed) {
        FixCaseUncleIsCommy(tree);
      } else {
        FixCaseUncleIsBlack(tree);
      }
    };

    void FixCaseUncleIsCommy(RedBlackTree *tree) {
      Node *uncle = Uncle();
      parent_->color_ = Color::kBlack;
      uncle->color_ = Color::kBlack;
      Node *granny = Granny();
      granny->color_ = Color::kRed;
      granny->AfterInsertFix(tree);
    };

    void FixCaseUncleIsBlack(RedBlackTree *tree) {
      Node *granny = Granny();
      if (granny && this == parent_->right_ && parent_ == granny->left_) {
        parent_->RotationLeft(tree);
        FixCaseRollingUncleAgain(tree);
        return;
      } else if (granny && this == parent_->left_ &&
                 parent_ == granny->right_) {
        parent_->RotationRight(tree);
        FixCaseRollingUncleAgain(tree);
        return;
      }
      parent_->FixCaseRollingUncleAgain(tree);
    };

    void FixCaseRollingUncleAgain(RedBlackTree *tree) {
      color_ = Color::kBlack;
      parent_->color_ = Color::kRed;
      if (this == parent_->left_) {
        parent_->RotationRight(tree);
      } else {
        parent_->RotationLeft(tree);
      }
    };

    void AfterDeleteFix(RedBlackTree *tree) {
      if (parent_ && this != tree->root_ && color_ == Color::kBlack) {
        FixCaseBroIsCommy(tree);
        FixCaseBroIsBlackFamily(tree);
      }
      color_ = Color::kBlack;
    };

    void FixCaseBroIsCommy(RedBlackTree *tree) {
      Node *bro = Brother();
      if (bro && bro->color_ == Color::kRed) {
        bro->color_ = Color::kBlack;
        parent_->color_ = Color::kRed;
        if (!parent_->left_ || this == parent_->left_) {
          parent_->RotationLeft(tree);
        } else {
          parent_->RotationRight(tree);
        }
      }
    };

    void FixCaseBroIsBlackFamily(RedBlackTree *tree) {
      Node *bro = Brother();
      if (bro && bro->color_ == Color::kBlack &&
          (!bro->left_ || bro->left_->color_ == Color::kBlack) &&
          (!bro->right_ || bro->right_->color_ == Color::kBlack)) {
        bro->color_ = Color::kRed;
        parent_->AfterDeleteFix(tree);
      } else {
        FixCaseBroIsHalfBlackFamily(tree);
      }
    };

    void FixCaseBroIsHalfBlackFamily(RedBlackTree *tree) {
      Node *bro = Brother();
      if (!parent_->left_ || this == parent_->left_) {
        if (bro && bro->color_ == Color::kBlack &&
            (bro->left_ && bro->left_->color_ == Color::kRed)) {
          bro->left_->color_ = Color::kBlack;
          bro->color_ = Color::kRed;
          bro->RotationRight(tree);
        }
      } else {
        if (bro && bro->color_ == Color::kBlack &&
            (bro->right_ && bro->right_->color_ == Color::kRed)) {
          bro->right_->color_ = Color::kBlack;
          bro->color_ = Color::kRed;
          bro->RotationLeft(tree);
        }
      }
      bro = Brother();
      bro->color_ = parent_->color_;
      parent_->color_ = Color::kBlack;
      if (!parent_->left_ || this == parent_->left_) {
        parent_->RotationLeft(tree);
        bro->right_->color_ = Color::kBlack;
      } else {
        parent_->RotationRight(tree);
        bro->left_->color_ = Color::kBlack;
      }
    };

    Node *parent_ = nullptr, *left_ = nullptr, *right_ = nullptr;
    Color color_ = Color::kRed;
  };

  template <typename T>
  class RBTreeIterator {
   public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef int32_t difference_type;
    typedef T *pointer;
    typedef T &reference;

    RBTreeIterator() : ptr_(nullptr), last_(nullptr){};
    RBTreeIterator(T *ptr) : ptr_(ptr) {
      if (ptr_) last_ = ptr_->Lower();
    };

    RBTreeIterator &operator++() {
      last_ = ptr_;
      ptr_ = ptr_->Bigger();
      return *this;
    }
    RBTreeIterator &operator--() {
      ptr_ = last_;
      if (ptr_) last_ = ptr_->Lower();
      return *this;
    }

    bool operator==(const RBTreeIterator &other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const RBTreeIterator &other) const {
      return ptr_ != other.ptr_;
    }

    T &operator*() const { return *ptr_; }
    T *operator->() const { return ptr_; }

    operator RBTreeIterator<const T>() const {
      return RBTreeIterator<const T>(ptr_, last_);
    }

   protected:
    T *ptr_;
    T *last_;
  };

  template <typename T>
  class RBTreeKeyIterator : public RBTreeIterator<T> {
   public:
    typedef RBTreeIterator<T> base;
    typedef typename RBTreeIterator<T>::iterator_category iterator_category;
    typedef T value_type;
    typedef int32_t difference_type;
    typedef T *pointer;
    typedef T &reference;

    RBTreeKeyIterator() : base(){};
    RBTreeKeyIterator(T *ptr) : base(ptr){};

    RBTreeKeyIterator &operator++() {
      base::operator++();
      return *this;
    }
    RBTreeKeyIterator &operator--() {
      base::operator--();
      return *this;
    }

    bool operator==(const RBTreeKeyIterator &other) const {
      return base::ptr_ == other.ptr_;
    }
    bool operator!=(const RBTreeKeyIterator &other) const {
      return base::ptr_ != other.ptr_;
    }

    typename std::conditional<std::is_const<T>::value, V, V &>::type
    operator*() {
      return base::ptr_->value;
    }

    operator RBTreeKeyIterator<const T>() const {
      return RBTreeKeyIterator<const T>(base::ptr_, base::last_);
    }
  };

  typedef RBTreeIterator<Node> iterator;
  typedef RBTreeIterator<const Node> const_iterator;
  typedef RBTreeKeyIterator<Node> key_iterator;
  typedef RBTreeKeyIterator<const Node> const_key_iterator;

  RedBlackTree() : root_(nullptr), count_(0){};
  RedBlackTree(K key, V value) : root_(new Node(key, value)), count_(1){};
  RedBlackTree(const RedBlackTree &other)
      : root_(new Node(other.root_->key, other.root_->value)),
        count_(other.count_) {
    root_->CopyHelper(other.root_);
  }
  RedBlackTree(RedBlackTree &&other) : RedBlackTree() {
    *this = std::move(other);
  }
  RedBlackTree &operator=(const RedBlackTree &other) {
    RedBlackTree cpy(other);
    *this = std::move(cpy);
    return *this;
  }
  RedBlackTree &operator=(RedBlackTree &&other) {
    std::swap(root_, other.root_);
    std::swap(count_, other.count_);
    return *this;
  }
  ~RedBlackTree() { delete root_; };

  void insert(K key, V value) {
    count_++;
    Node *n_node = new Node(key, value);
    if (!root_) {
      root_ = n_node;
      root_->color_ = Color::kBlack;
      return;
    }
    Node *deep = root_;
    while (deep != nullptr) {
      if (deep->key <= key) {
        if (deep->right_ == nullptr) {
          n_node->parent_ = deep;
          deep->right_ = n_node;
          break;
        }
        deep = deep->right_;
      } else {
        if (deep->left_ == nullptr) {
          n_node->parent_ = deep;
          deep->left_ = n_node;
          break;
        }
        deep = deep->left_;
      }
    }
    if (!n_node->Granny()) return;
    n_node->AfterInsertFix(this);
    root_->color_ = Color::kBlack;
  };
  void remove(K key) {
    Node *deep = root_->FindFirstOf(key);
    count_--;
    if (!deep) {
      return;
    }
    Color color_of_del = deep->color_;
    Node *replace = nullptr;
    if (!deep->right_) {
      replace = deep->Swap(deep->left_, this);
    } else if (!deep->left_) {
      replace = deep->Swap(deep->right_, this);
    } else {
      // заменить удаляемое на число, которое max из чисел меньше удаляемого
      // или min из чисел больше удаляемого
      Node *max = deep->left_->Max();
      if (max->parent_->left_ == max) {
        max->parent_->left_ = nullptr;
      } else {
        max->parent_->right_ = nullptr;
      }
      deep->Swap(max, this);
      replace = deep;
      color_of_del = max->color_;
      max->color_ = deep->color_;
      deep->color_ = color_of_del;
    }

    if (color_of_del == Color::kBlack) {
      replace->AfterDeleteFix(this);
    }
    deep->parent_ = deep->left_ = deep->right_ = nullptr;
    delete deep;
  };

  void print() const {
    if (root_) {
      root_->PrintHelper("", true, 5);
    }
  };

  iterator begin() {
    if (root_) return iterator(root_->Min());
    return end();
  }
  iterator end() {
    if (root_) return ++iterator(root_->Max());
    return iterator();
  }

  const_iterator begin() const {
    if (root_) return const_iterator(root_->Min());
    return end();
  }
  const_iterator end() const {
    if (root_) return ++const_iterator(root_->Max());
    return const_iterator();
  }

  key_iterator kbegin() {
    if (root_) return key_iterator(root_->Min());
    return kend();
  }
  key_iterator kend() {
    if (root_) return ++key_iterator(root_->Max());
    return key_iterator();
  }

  const_key_iterator kbegin() const {
    if (root_) return const_key_iterator(root_->Min());
    return kend();
  }
  const_key_iterator kend() const {
    if (root_) return ++const_key_iterator(root_->Max());
    return const_key_iterator();
  }

  Node *root_;
  size_t count_;
};
}  // namespace s21

#endif  // SRC_CONTAINERS_S21_RED_BLACK_TREE_H_
