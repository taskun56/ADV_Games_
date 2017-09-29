#include <iostream>
#include "state_machine.h"
#include "enemy.h"

using namespace std;

void enter_work(enemy& e)
{
    cout << "Entered work state" << endl;
}

void do_work(enemy &e)
{
    cout << "Doing work" << endl;
}

void exit_work(enemy &e)
{
    cout << "Exiting work state" << endl;
}

void enter_rest(enemy &e)
{
    cout << "Entered rest state" << endl;
}

void do_rest(enemy &e)
{
    cout << "Resting" << endl;
}

void exit_rest(enemy &e)
{
    cout << "Exiting rest state" << endl;
}

int main(int argc, char **argv)
{
    enemy e;
    e.sm.add_state("work", enter_work, do_work, exit_work);
    e.sm.add_state("rest", enter_rest, do_rest, exit_rest);
    e.sm.change_state("rest", e);
    for (int i = 0; i < 10; ++i)
        e.update();
    e.sm.change_state("work", e);
    for (int i = 0; i < 10; ++i)
        e.update();
    e.sm.change_state("rest", e);
    for (int i = 0; i < 10; ++i)
        e.update();
    return 0;
}