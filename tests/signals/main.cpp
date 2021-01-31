#include "Signal.h"
#include "Property.h"
#include <iostream>
#include <vector>

using namespace coral;

class my_class
{
public:
  void on_update(int test) { std::cout << test << std::endl; }

  PropertyArray<std::vector<int>> test2;
};

int main()
{
    my_class c;

    Signal<int> signal;
    signal.connect([](int test){ std::cout << test << std::endl; });
    signal.connect<&my_class::on_update>(&c);
    signal.emit(12);

    c.test2.changed.connect([](const std::vector<int>& test){ std::cout << test[0] << std::endl; });
    c.test2.set(5, 0);

}