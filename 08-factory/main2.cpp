#include <iostream>
#include <string>
#include "generic_factory.h"

using namespace std;

class temp : public generic_factory<string, string, int>
{
};

string to_str(int n)
{
    return to_string(n);
}

int main(int argc, char **argv)
{
    temp t;
    t.reg("Boo", to_str);
    auto str = t.create("Boo", 5);
    cout << str << endl;
    return 0;
}