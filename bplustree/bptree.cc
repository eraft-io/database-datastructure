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