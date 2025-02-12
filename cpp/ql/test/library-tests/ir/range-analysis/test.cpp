#include "test_util.h"
  int f1(int x, int y) {
    if (x < 500) {
      if (x > 400) {
        range(x);  // $ range=>=401 range=<=499
        return x;
      }

      if (y - 2 == x && y > 300) { // $ overflow=-
        range(x + y);  // $ range=<=802 range=>=600
        return x + y;
      }

      if (x != y + 1) { // $ overflow=+
        range(x);  // $ range=<=400
        int sum = x + y; // $ overflow=+-
      } else {
        if (y > 300) {
          range(x); // $ range=>=302 range=<=400 range="<=InitializeParameter: y+1" MISSING: range===y+1
          range(y); // $ range=>=301 range=<=399 range="==InitializeParameter: x | Store: x-1"
          int sum = x + y;
        }
      }

      if (x > 500) {
        range(x); // $ range=<=400 range=>=501
        return x;
      }
    }

    return 0;
  }

  int f2(int x, int y, int z) {
    if (x < 500) {
      if (x > 400) {
        range(x);  // $ range=>=401 range=<=499
        return x;
      }

      if (y == x - 1 && y > 300 && y + 2 == z && z == 350) { // $ overflow=+ overflow=-
        range(x);  // $ range===349 range="==InitializeParameter: y+1" range="==InitializeParameter: z-1"
        range(y);  // $ range===348 range=">=InitializeParameter: x | Store: x-1" range="==InitializeParameter: z-2" MISSING: range===x-1 
        range(z);  // $ range===350 range="<=InitializeParameter: y+2" MISSING: range===x+1 range===y+2
        return x + y + z;
      }
    }

    return 0;
  }

  void* f3_get(int n);

  void f3() {
    int n = 0;
    while (f3_get(n)) n+=2;

    for (int i = 0; i < n; i += 2) {
      range(i); // $ range=>=0 range="<=Phi: call to f3_get-2"
    }
  }

int f4(int x) {
  for (int i = 0; i <= 100; i++) {
    range(i); // $ range=<=100 range=>=0
    if(i == 100) {
      range(i); // $ range===100
    } else {
      range(i); // $ range=<=99 range=>=0
    }
  }
}

// No interesting ranges to check here - this irreducible CFG caused an infinite loop due to back edge detection
void gotoLoop(bool b1, bool b2)
{
  int j;

  if (b1)
    return;

  if (!b2)
  {
    for (j = 0; j < 10; ++j)
    {
     goto main_decode_loop;
    }
  }
  else
  {
    for (j = 0; j < 10; ++j)
    {
      int x;
      main_decode_loop:
    }
  }
}

void test_sub(int x, int y, int n) {
  if(x > 0 && x < 500) {
    if(y > 0 && y < 10) {
      range(x - y); // $ range=<=498 range=>=-8
    }

    if(n > 0 && n < 100) {
      for (int i = 0; i < n; i++)
      {
        range(n - i); // $ range=">=Phi: i-97" range=<=99 range=>=-97
        range(i - n); // $ range="<=Phi: i-1" range=">=Phi: i-99" range=<=97 range=>=-99 
      }

      for (int i = n; i != 0; i--)
      {
        range(n - i); // $ SPURIOUS: overflow=+
        range(i - n); // $ range=">=Phi: i-99"
      }
    }
  }
}

void test_div(int x) {
  if (3 <= x && x <= 7) {
    range(x / 2); // $ range=>=1 range=<=3
    range(x / 3); // $ range=>=1 range=<=2
    range(x >> 2); // $ range=>=0 range=<=1
  }
  if (2 <= x && x <= 8) {
    range(x / 2); // $ range=>=1 range=<=4
    range(x / 3); // $ range=>=0 range=<=2
    range(x >> 2); // $ range=>=0 range=<=2
  }
}
