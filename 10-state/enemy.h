#pragma once

#include "state_machine.h"

class enemy
{
public:
    // We will keep it simple again.
    state_machine<enemy> sm;

    void update()
    {
        sm.update(*this);
    }
};