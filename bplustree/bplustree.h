// MIT License

// Copyright (c) 2021 ERaftGroup

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_

#include <iostream>
#include <queue>

template <typename k, typename v>
class BPTree;

template <typename k, typename v>
class BPTNode {
  // kyes values next&pre node
  k* keys_;
  v* values_;
  BPTNode<k, v>* next_;
  BPTNode<k, v>* pre_;
  // minimum degreee
  int degree_;
  int halfdegree_;
  // child pointers
  BPTNode<k, v>** child_;
  // number of keys
  int keycount_;
  // Is true when node is leaf
  bool isleaf_;
  // find the index of the key which is greater than or equal to key
  int Findkey(k key);
  // get the smallest key of the leaf node
  k GetMin(BPTNode<k, v>* node) const;
  // Remove the key which is in a leaf node
  void RemoveFromLeaf(int idx);
  // Fill up th child node
  void Fill(int idx);
  // borrow a key from the child_[idx-1]th node and place it in child_[idx]
  void BorrowFromPrev(int idx);
  // borrow a key from the child_[idx+1]th node and place it in child_[idx]
  void BorrowFromNext(int idx);
  // Merge idx-th child with idx+1 child
  void Merge(int idx);

 public:
  BPTNode(int degree, bool leaf);
  ~BPTNode() {}
  void Insert(k key, v value);
  void SplitChild(int index, BPTNode<k, v>* node);
  void Display();
  BPTNode<k, v>* Search(k key);
  void Remove(k key);

  friend class BPTree<k, v>;
};

template <typename k, typename v>
class BPTree {
  BPTNode<k, v>* root_;
  BPTNode<k, v>* header_;
  int degree_;
  int halfdegree_;

 public:
  BPTree(int degree) : root_(nullptr), header_(nullptr), degree_(degree) {}
  ~BPTree() {}
  void Display();
  void Displaybylink();
  BPTNode<k, v>* Search(k key) {
    return (root_ != nullptr) ? root_->Search(key) : nullptr;
  }
  void Insert(k key, v value);
  void Remove(k key);
};

template <typename k, typename v>
BPTNode<k, v>::BPTNode(int degree, bool leaf) {
  degree_ = degree;
  halfdegree_ = degree_ % 2 == 1 ? degree_ / 2 : degree_ / 2 - 1;
  isleaf_ = leaf;
  keys_ = new k[degree_];
  next_ = pre_ = nullptr;
  // only leafnode store data
  if (isleaf_) {
    values_ = new v[degree_];
    child_ = nullptr;
  } else {
    child_ = new BPTNode<k, v>*[degree_];
  }
  keycount_ = 0;
}

template <typename k, typename v>
int BPTNode<k, v>::Findkey(k key) {
  int index = 0;
  while (index < keycount_ && keys_[index] <= key) {
    ++index;
  }
  return index;
}

template <typename k, typename v>
k BPTNode<k, v>::GetMin(BPTNode<k, v>* node) const {
  while (!node->isleaf_) {
    node = node->child_[0];
  }
  return node->keys_[0];
}

template <typename k, typename v>
void BPTNode<k, v>::RemoveFromLeaf(int idx) {
  for (int i = idx + 1; i < keycount_; ++i) {
    keys_[i - 1] = keys_[i];
    values_[i - 1] = values_[i];
  }
  keycount_ -= 1;
}

template <typename k, typename v>
void BPTNode<k, v>::Fill(int idx) {
  // borrow a key from prev or next
  if (idx != 0 && child_[idx - 1]->keycount_ > halfdegree_) {
    BorrowFromPrev(idx);
  } else if (idx != keycount_ && child_[idx + 1]->keycount_ > halfdegree_) {
    BorrowFromNext(idx);
  } else {
    if (idx != 0) {
      Merge(idx - 1);
    } else {
      Merge(idx);
    }
  }
}

template <typename k, typename v>
void BPTNode<k, v>::BorrowFromPrev(int idx) {
  BPTNode<k, v>* child = child_[idx];
  BPTNode<k, v>* sibling = child_[idx - 1];
  // Move all keys in C[idx] one step backward
  for (int i = child->keycount_ - 1; i >= 0; --i) {
    child->keys_[i + 1] = child->keys_[i];
    if (child->isleaf_) {
      child->values_[i + 1] = child->values_[i];
    }
  }
  // If C[idx] is not a leaf, move all its child pointers one step backword
  if (!child->isleaf_) {
    for (int i = child->keycount_; i >= 0; --i) {
      child->child_[i + 1] = child->child_[i];
    }
  }
  child->keys_[0] = sibling->keys_[sibling->keycount_ - 1];
  if (child->isleaf_) {
    child->values_[0] = sibling->values_[sibling->keycount_ - 1];
  }
  if (!child->isleaf_) {
    child->child_[0] = sibling->child_[sibling->keycount_];
  }
  keys_[idx - 1] = sibling->keys_[sibling->keycount_ - 1];
  child->keycount_ += 1;
  sibling->keycount_ -= 1;
}

template <typename k, typename v>
void BPTNode<k, v>::BorrowFromNext(int idx) {
  BPTNode<k, v>* child = child_[idx];
  BPTNode<k, v>* sibling = child_[idx + 1];
  child->keys_[child->keycount_] = keys_[idx];
  if (!child->isleaf_) {
    child->child_[child->keycount_ + 1] = sibling->child_[0];
  } else {
    child->values_[child->keycount_] = sibling->values_[0];
  }
  if (child->keycount_ == 0 && idx != 0) {
    keys_[idx - 1] = sibling->keys_[0];
  }
  for (int i = 1; i < sibling->keycount_; ++i) {
    sibling->keys_[i - 1] = sibling->keys_[i];
    if (sibling->isleaf_) {
      sibling->values_[i - 1] = sibling->values_[i];
    }
  }
  if (!sibling->isleaf_) {
    for (int i = 1; i < sibling->keycount_; ++i) {
      sibling->child_[i - 1] = sibling->child_[i];
    }
  }
  keys_[idx] = sibling->keys_[0];
  child->keycount_ += 1;
  sibling->keycount_ -= 1;
}

template <typename k, typename v>
void BPTNode<k, v>::Merge(int idx) {
  BPTNode<k, v>* leftchild = child_[idx];
  BPTNode<k, v>* rightchild = child_[idx + 1];
  if (!leftchild->isleaf_) {
    // copy rightchild's data to leftchild
    leftchild->child_[leftchild->keycount_ + 1] = rightchild->child_[0];
    for (int i = 1; i < rightchild->keycount_; ++i) {
      leftchild->child_[i + leftchild->keycount_ + 1] = rightchild->child_[i];
    }
    leftchild->keys_[leftchild->keycount_] = keys_[idx];
    for (int i = 0; i < rightchild->keycount_; ++i) {
      leftchild->keys_[i + leftchild->keycount_ + 1] = rightchild->keys_[i];
    }
    leftchild->keycount_ += rightchild->keycount_ + 1;
    delete (rightchild);
  } else {
    for (int i = 0; i < rightchild->keycount_; ++i) {
      leftchild->keys_[i + leftchild->keycount_] = rightchild->keys_[i];
      leftchild->values_[i + leftchild->keycount_] = rightchild->values_[i];
    }
    leftchild->keycount_ += rightchild->keycount_;
    if (rightchild->next_ != nullptr) {
      leftchild->next_ = rightchild->next_;
      rightchild->next_->pre_ = leftchild;
    } else {
      leftchild->next_ = nullptr;
    }
  }
  for (int i = idx + 1; i < keycount_; ++i) {
    keys_[i - 1] = keys_[i];
  }
  for (int i = idx + 2; i <= keycount_; ++i) {
    child_[i - 1] = child_[i];
  }
  keycount_--;
}

template <typename k, typename v>
void BPTNode<k, v>::Remove(k key) {
  // find the index of the key which is greater than or equal to key
  int index = Findkey(key);
  if (keys_[index - 1] == key && isleaf_) {
    if (isleaf_) {
      RemoveFromLeaf(index - 1);
    }
  } else {
    child_[index]->Remove(key);
    if (index != 0) {
      keys_[index - 1] = GetMin(child_[index]);
    }
    if (child_[index]->keycount_ < halfdegree_) {
      Fill(index);
    }
  }
}

template <typename k, typename v>
void BPTNode<k, v>::Display() {
  for (int i = 0; i < keycount_; ++i) {
    std::cout << "key:" << keys_[i] << " ";
    if (isleaf_) {
      std::cout << "value:" << values_[i] << " ";
    }
  }
  std::cout << "\n";
}
template <typename k, typename v>
void BPTNode<k, v>::Insert(k key, v value) {
  int i = keycount_ - 1;
  if (isleaf_ == true) {
    // find the location of new key to be Inserted
    // Move all the larger keys to one position behind
    while (i >= 0 && keys_[i] > key) {
      keys_[i + 1] = keys_[i];
      values_[i + 1] = values_[i];
      --i;
    }
    // Insert the new key at found lacation
    keys_[i + 1] = key;
    values_[i + 1] = value;
    keycount_++;
  } else {
    while (i >= 0 && keys_[i] > key) {
      --i;
    }
    child_[i + 1]->Insert(key, value);
    if (child_[i + 1]->keycount_ == degree_) {
      SplitChild(i + 1, child_[i + 1]);
    }
  }
}

template <typename k, typename v>
void BPTNode<k, v>::SplitChild(int index, BPTNode<k, v>* node) {
  // create a new node which is going to store last (degree - halfdegree) keys
  // of node node->kyes_ [0, halfdegree-1], [halfdegree, degree-1]
  //                      | - - - - - |     | - - - - - |
  BPTNode<k, v>* temp = new BPTNode<k, v>(node->degree_, node->isleaf_);
  int lastkeycount = degree_ - degree_ / 2;
  if (node->isleaf_) {
    temp->keycount_ = lastkeycount;
    // copy the last (degree - halfdegree) keys of node to temp
    for (int i = 0; i < lastkeycount; ++i) {
      temp->keys_[i] = node->keys_[i + degree_ / 2];
      temp->values_[i] = node->values_[i + degree_ / 2];
    }
    // link leaf nodes
    temp->next_ = node->next_;
    node->next_ = temp;
    temp->pre_ = node;
    if (temp->next_ != nullptr) temp->next_->pre_ = temp;
  } else {
    temp->keycount_ = lastkeycount - 1;
    // copy the [halfdegree+1, degree]keys of node to temp
    for (int i = 0; i < lastkeycount - 1; ++i) {
      temp->keys_[i] = node->keys_[i + halfdegree_ + 1];
    }
    // copy the [halfdegree, degree] children of node to temp
    for (int i = 0; i < lastkeycount; ++i) {
      temp->child_[i] = node->child_[i + halfdegree_ + 1];
    }
  }
  node->keycount_ = degree_ / 2;
  // Move all the larger keys to one position behind
  for (int i = keycount_; i >= index + 1; --i) {
    child_[i + 1] = child_[i];
  }
  // Insert the new child in the reserved position
  child_[index + 1] = temp;
  // Move all the larger keys to one position behind
  for (int i = keycount_ - 1; i >= index; --i) {
    keys_[i + 1] = keys_[i];
  }
  // copy the middle key of node to this node
  keys_[index] = node->keys_[degree_ / 2];
  // increment keycount
  keycount_ += 1;
}

template <typename k, typename v>
BPTNode<k, v>* BPTNode<k, v>::Search(k key) {
  int i = 0;
  while (i < keycount_ && key > keys_[i]) {
    ++i;
  }
  if (key == keys_[i] && isleaf_ == true) {
    std::cout << "find it! key: " << key << "value:" << values_[i] << std::endl;
    return this;
  }
  if (isleaf_ == true) {
    std::cout << "not find " << key << std::endl;
    return nullptr;
  }
  return child_[i]->Search(key);
}

template <typename k, typename v>
void BPTree<k, v>::Insert(k key, v value) {
  if (root_ == nullptr) {
    root_ = new BPTNode<k, v>(degree_, true);
    root_->keys_[0] = key;
    root_->values_[0] = value;
    root_->keycount_ = 1;
    header_ = root_;
  } else {
    root_->Insert(key, value);
    if (root_->keycount_ == degree_) {
      BPTNode<k, v>* temp = new BPTNode<k, v>(degree_, false);
      temp->child_[0] = root_;
      temp->SplitChild(0, root_);
      root_ = temp;
    }
  }
}

template <typename k, typename v>
void BPTree<k, v>::Remove(k key) {
  if (root_ == nullptr) {
    std::cout << "The tree is empty\n";
    return;
  }
  root_->Remove(key);
  if (root_->keycount_ == 0) {
    BPTNode<k, v>* temp = root_;
    if (root_->isleaf_) {
      delete root_;
      root_ = nullptr;
    } else {
      root_ = root_->child_[0];
      delete temp;
    }
  }
}

template <typename k, typename v>
void BPTree<k, v>::Display() {
  if (root_ != nullptr) {
    std::cout << "-------- Display BPTree ------\n";
    std::queue<BPTNode<k, v>*> q;
    int level = 1;
    q.push(root_);
    while (!q.empty()) {
      int len = q.size();
      std::cout << "------- level " << level++ << " ------\n";
      for (int i = 0; i < len; ++i) {
        BPTNode<k, v>* temp = q.front();
        q.pop();
        std::cout << " " << i << ":";
        temp->Display();
        if (!temp->isleaf_)
          for (int j = 0; j <= temp->keycount_; ++j) {
            q.push(temp->child_[j]);
          }
      }
      std::cout << std::endl;
    }
  } else {
    std::cout << "BPTree is empty!\n";
  }
}

template <typename k, typename v>
void BPTree<k, v>::Displaybylink() {
  if (header_ != nullptr) {
    BPTNode<k, v>* temp = header_;
    std::cout << "-------- Display BPTree ------\n";
    while (temp != nullptr) {
      temp->Display();
      temp = temp->next_;
    }
  }
}
#endif