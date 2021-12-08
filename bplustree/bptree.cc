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

#include "bplustree.h"
// #include "bpt.h"

int main() {
  BPTree<int, int> bpt(3);
  int num[10] = {1, 2, 4, 5, 7, 8, 9, 11, 3};
  for (int i = 0; i < 9; ++i) {
    bpt.Insert(num[i], i);
    bpt.Display();
  }
  //   bpt.Insert(-1, -1);
  //   bpt.Insert(1, 1);
  // bpt.Display();
  std::cout << "---------delete----------|||||||------\n";
  bpt.Remove(7);
  bpt.Display();
  bpt.Remove(8);
  bpt.Display();
  bpt.Remove(5);
  bpt.Display();
  bpt.Remove(1);
  bpt.Display();
  // bpt.Displaybylink();
  bpt.Remove(2);
  bpt.Display();
  return 0;
}