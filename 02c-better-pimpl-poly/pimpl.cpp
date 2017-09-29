#include <iostream>
#include <memory>

using namespace std;

class base
{
protected:
    struct base_impl
    {
        float x = 5.0f;
        float y = 10.0f;
        float z = 20.0f;

        virtual void print()
        {
            cout << x << " " << y << " " << z << endl;
        }
    };

    shared_ptr<base_impl> _self = nullptr;

    // Protected constructor taking an existing self pointer
    base(shared_ptr<base_impl> self)
    : _self(self)
    {
    }
public:
    base()
    : _self{make_shared<base_impl>()}
    {
    }

    // Note that print at this level is no longer virtual
    void print()
    {
        _self->print();
    }
};

class derived : public base
{
protected:
    // Only focus on impl changing.
    struct derived_impl : public base::base_impl
    {
        float r = 1.0f;
        float g = 2.0f;
        float b = 3.0f;

        // Declare as override and final (no more overrides).  Helps the compiler.
        virtual void print() override final
        {
            cout << x << " " << y << " " << z << endl;
            cout << r << " " << g << " " << b << endl;
        }
    };

    // No pointer here as we don't need it.
    // However, you might need it if you want to access unique members.

public:
    derived()
    : base{make_shared<derived_impl>()}
    {
    }

    // No need to implement print.  Just use the base one.
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