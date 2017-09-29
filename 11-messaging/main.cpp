#include <iostream>
#include "messenger.h"

using namespace std;

class entity : public message_handler<entity>
{
private:
    std::string _name = "";
public:
    entity(const std::string &name)
    : _name(name)
    {
    }

    void print()
    {
        cout << _name << endl;
    }

    void update()
    {
        execute(*this);
    }
};

void do_work(entity &e)
{
    e.print();
}

void do_sleep(entity &e)
{
    e.print();
    cout << "....sleeping" << endl;
}

int main(int argc, char **argv)
{
    entity sam("Sam");
    entity kevin("Kevin");
    entity gregory("Gregory");

    mediator<entity> med;
    med.add_handler("Sam", entity("Sam"));
    med.add_handler("Kevin", entity("Kevin"));
    med.add_handler("Gregory", entity("Gregory"));

    med.send_message("Sam", command<entity>(do_work));
    med.send_message("Sam", command<entity>(do_work));
    med.send_message("Kevin", command<entity>(do_sleep));
    med.send_message("Gregory", command<entity>(do_sleep));

    sam.update();
    kevin.update();
    gregory.update();

    return 0;
}