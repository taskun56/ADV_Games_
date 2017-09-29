#include <iostream>
#include "physics_system.h"

using namespace std;

int main(int argc, char **argv)
{
    physics_system sys;
    auto c1 = sys.create("FUN");
    auto c2 = sys.create("METHOD");
    return 0;
}