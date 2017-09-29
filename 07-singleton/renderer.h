#pragma once

#include <iostream>
#include "singleton.h"

// We use metatemplating "magic".  singleton needs a
// type, so we pass it renderer.  renderer now inherits
// from singleton of type renderer.  This works.  Templating
// in C++ is very powerful.
class renderer : public singleton<renderer>
{
    // We need singleton to be a friend to call the constructor
    friend class singleton<renderer>;
private:
    // Constructor is private because it can be.
    renderer()
    {
    }

    // Good practice is to delete all other constructors
    // and assignment operators.
    renderer(const renderer &other) = delete;
    renderer(renderer &&rhs) = delete;
    renderer& operator=(const renderer &other) = delete;
    renderer& operator=(renderer &&rhs) = delete;
public:
    // Destructor still needs to be public.
    ~renderer() = default;

    // Some methods to call
    void draw() { std::cout << "Called draw" << std::endl; }
    void clear() { std::cout << "Called clear" << std::endl; }
};