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

#ifndef LRUCACHE_H_
#define LRUCACHE_H_

#include <iostream>
#include <mutex>

template <typename K, typename V>
struct Result {
  K key;
  V value;
  bool error;
  Result(K k, bool e) {
    key = k;
    error = e;
  }
};

template <typename K, typename V>
class LRUCache {
 private:
  struct Data {
    K key;
    V value;
    Data* next;
    Data* pre;
    Data() { next = pre = nullptr; }
  };
  Data* header_;
  Data* tail_;
  int lenght_;
  int occupied_;
  std::mutex mtx_;

 public:
  LRUCache(int len);
  ~LRUCache();
  Result<K, V> Get(K);
  void Set(K, V);
  void ShowAll() const;
};

template <typename K, typename V>
LRUCache<K, V>::LRUCache(int len) {
  lenght_ = len;
  header_ = new Data();
  Data* temp = header_;
  for (int i = 0; i < lenght_ - 1; ++i) {
    Data* newdata = new Data();
    newdata->pre = temp;
    temp->next = newdata;
    temp = temp->next;
    if (i == lenght_ - 2) {
      header_->pre = newdata;
      newdata->next = header_;
    }
  }
  tail_ = header_->pre;
  occupied_ = 0;
}

template <typename K, typename V>
Result<K, V> LRUCache<K, V>::Get(K key) {
  Data* temp = header_;
  for (int i = 0; i < occupied_; ++i) {
    if (temp->key == key) {
      Result<K, V> result(key, 0);
      result.value = temp->value;
      if (temp == header_) return result;
      //链接当前节点的前后节点，将当前节点单独拿出来准备放到header_
      temp->next->pre = temp->pre;
      temp->pre->next = temp->next;
      //将temp插入到header前，temp变成header
      temp->pre = header_->pre;
      temp->pre->next = temp;
      header_->pre = temp;
      temp->next = header_;
      header_ = temp;
      tail_ = header_->pre;
      return result;
    }
    temp = temp->next;
  }
  std::cout << "Not find " << key << "\n";
  return Result<K, V>(key, 1);
}

template <typename K, typename V>
void LRUCache<K, V>::Set(K key, V value) {
  Data* temp = header_;
  for (int i = 0; i < occupied_; ++i) {
    //如果已经在cache中
    if (temp->key == key) {
      std::unique_lock<std::mutex> lock(mtx_);
      temp->value = value;
      if (temp == header_) return;
      //链接当前节点的前后节点，将当前节点单独拿出来准备放到header_
      temp->next->pre = temp->pre;
      temp->pre->next = temp->next;
      //将temp插入到header前，temp变成header
      temp->pre = header_->pre;
      temp->pre->next = temp;
      header_->pre = temp;
      temp->next = header_;
      header_ = temp;
      tail_ = header_->pre;
      return;
    }
    temp = temp->next;
  }
  //将最后一个替换
  std::unique_lock<std::mutex> lock(mtx_);
  tail_->key = key;
  tail_->value = value;
  header_ = tail_;
  tail_ = tail_->pre;
  //如果当前缓存未满，增增加一个已经使用的记录
  if (occupied_ < lenght_) {
    occupied_++;
  }
}
template <typename K, typename V>
void LRUCache<K, V>::ShowAll() const {
  Data* temp = header_;
  for (int i = 0; i < occupied_; ++i) {
    std::cout << "key:" << temp->key << " value:" << temp->value << "\n";
    temp = temp->next;
  }
  std::cout << "\n";
}
template <typename K, typename V>
LRUCache<K, V>::~LRUCache() {
  for (int i = 0; i < lenght_; ++i) {
    Data* temp = header_;
    header_ = header_->next;
    delete temp;
  }
}
#endif  // LRUCACHE_H_