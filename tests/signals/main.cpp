#include "Logs.h"
#include "Property.h"
#include "Signal.h"
#include <chrono>
#include <iostream>
#include <vector>

using namespace coral;
using namespace std::chrono;

class my_class
{
public:
  void on_update(int test) { std::cout << test << std::endl; }

  PropertyArray<int> test2;
};

int main()
{
    my_class c;

    Signal<int> signal;
    connect<&my_class::on_update>(signal, &c);
    signal(12);

    Property<int> t;
    t.get();
}
