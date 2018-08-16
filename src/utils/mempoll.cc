#ifndef __SRC_UTILS_LRUCACHE_H__
#define __SRC_UTILS_LRUCACHE_H__

#include <boost/pool/object_pool.hpp>
#include <iostream>

class Demo {
  
  public:
    int a, b;

    Demo(int x = 1, int y = 2) : a(x), b(y) {};

    // ~Demo();

};

#endif

using namespace std;
using namespace boost;

int main() {
  object_pool<Demo> p1;

  Demo * p = p1.malloc();

  cout << p1.is_from(p) << endl;
  
  cout << (p->a != 1) << "  " << (p->b != 2) << endl;

  Demo * pa = p1.construct(4, 5);

  cout << (pa->a == 4 && pa->b == 5) << endl;

  p1.destroy(p);

  cout << p1.is_from(p) << endl;
}