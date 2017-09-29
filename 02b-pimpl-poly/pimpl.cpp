#include <iostream>
#include <memory>

using namespace std;

class base
{
    // As we are deriving from this base type use protected definitions.
protected:
    struct data
    {
        float x = 5.0f;
        float y = 10.0f;
        float z = 20.0f;
    };

    shared_ptr<data> _self = nullptr;

    // Provide a method to set self pointer for derived classes
    void set_self(shared_ptr<data> self)
    {
        _self = self;
    }

public:
    base()
    : _self{std::make_shared<data>()}
    {
    }

    virtual void print()
    {
        cout << _self->x << " " << _self->y << " " << _self->z << endl;
    }
};

class derived : public base
{
protected:
    struct derived_data : public base::data
    {
        float r = 1.0f;
        float g = 2.0f;
        float b = 3.0f;
    };

    // Maintain a local pointer to data
    std::shared_ptr<derived_data> _self = nullptr;

public:
    derived()
    : _self{make_shared<derived_data>()}
    {
        set_self(_self);
    }

    // Declare as override and final (no more overrides).  Helps the compiler.
    virtual void print() override final
    {
        cout << _self->x << " " << _self->y << " " << _self->z << endl;
        cout << _self->r << " " << _self->g << " " << _self->b << endl;
    }
};

void do_work_copy(base obj)
{
    cout << "In do work with copy of a base" << endl;
    obj.print();
}

void do_work_ref(base &obj)
{
    cout << "In do work with a reference to a base" << endl;
    obj.print();
}

int main(int argc, char **argv)
{
    derived d;
    cout << "**************************" << endl;
    cout << "Calling print on derived" << endl;
    d.print();
    cout << "**************************" << endl;
    do_work_copy(d);
    cout << "**************************" << endl;
    do_work_ref(d);
    cout << "**************************" << endl;
    return 0;
}