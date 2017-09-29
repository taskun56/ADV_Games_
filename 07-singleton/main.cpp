#include <iostream>
#include "renderer.h"

using namespace std;

int main(int argc, char **argv)
{
    renderer::get().draw();
    renderer::get().clear();
    return 0;
}