#include <iostream>
#include <vector>

// This program illustrates object construction and destruction.  You need to 
// be aware when these calls are being made.

using namespace std;

class base
{
public:
    // Default constructor
    base() { cout << "Base Default Constructor called" << endl; }

    // Copy constructor
    base(const base &other) { cout << "Base Copy Constructor called" << endl; }

    // Move constructor.  If unsure look up move semantics.
    base(base &&rhs) { cout << "Base Move Constructor called" << endl; }

    // Destructor.  Note this is virtual as it is a base class.
    virtual ~base() { cout << "Base Destructor called" << endl; }

    // Copy assignment operator
    base& operator=(const base &other)
    {
        cout << "Base Copy Assignment Operator called" << endl;
        return *this;
    }

    // Move assignment operator
    base& operator=(base &&other)
    {
        cout << "Base Move Assignment Operator called" << endl;
        return *this;
    }
};

class derived : public base
{
public:
    // Default constructor
    derived() { cout << "Derived Default Constructor called" << endl; }

    // Another constructor.  We are calling a specific base constructor here.
    derived(int x) : base() { cout << "Derived Other Constructor called" << endl; }

    // Copy constructor
    derived(const derived &other) { cout << "Derived Copy Constructor called" << endl; }

    // Move constructor
    derived(derived &rhs) { cout << "Derived Move Constructor called" << endl; }

    // Destructor.
    virtual ~derived() { cout << "Derived Destructor called" << endl; }

    // Copy assignment operator
    derived& operator=(const derived &other)
    {
        cout << "Derived Copy Assignment Operator called" << endl;
        return *this;
    }

    // Move assignment operator
    derived& operator=(derived &&rhs)
    {
        cout << "Derived Move Assignment Operator called" << endl;
        return *this;
    }
};

// We will call these functions to illustrate construction calls

void foo_copy(derived obj)
{
    cout << "Inside foo with copy of derived" << endl;
}

void foo_ref(derived &obj)
{
    cout << "Inside foo with reference of derived" << endl;
}

void foo_move(derived &&obj)
{
    cout << "Inside foo with rvalue reference of derived" << endl;
}

int main(int argc, char **argv)
{
    cout << "**************************" << endl;
    cout << "Creating a new base" << endl;
    base b0;
    cout << "**************************" << endl;
    cout << "Creating a new derived" << endl;
    derived d0;
    cout << "**************************" << endl;
    cout << "Creating a derived with a value - note that braces construction now prefered" << endl;
    derived d1{5};
    cout << "**************************" << endl;
    cout << "Creating a derived from an existing derived" << endl;
    derived d2 = d0;
    cout << "**************************" << endl;
    cout << "Creating a new derived by passing in a derived parameter" << endl;
    derived d3(d0);
    cout << "**************************" << endl;
    cout << "Creating a reference to an existing derived" << endl;
    derived &d4 = d0;
    cout << "**************************" << endl;
    cout << "Creating a derived from a reference to a derived" << endl;
    derived d5 = d4;
    cout << "**************************" << endl;
    cout << "Creating a base reference from a derived" << endl;
    base &b1 = d4;
    cout << "**************************" << endl;
    cout << "Assigning an existing derive to another derive" << endl;
    d1 = d2;
    cout << "**************************" << endl;
    cout << "Calling foo with copy semantics" << endl;
    foo_copy(d0);
    cout << "**************************" << endl;
    cout << "Calling foo with reference semantics" << endl;
    foo_ref(d0);
    cout << "**************************" << endl;
    cout << "Calling foo with move semantics" << endl;
    foo_move(std::move(d1));
    cout << "**************************" << endl;
    cout << "Passing a new object either requires a copy or a move" << endl;
    foo_copy(derived());
    // foo_ref(derived()); - this line will not compile!
    foo_move(derived());
    cout << "**************************" << endl;
    cout << "Creating a vector of derived of a given size will call that number of constructors" << endl;
    vector<derived> d6(10);
    cout << "**************************" << endl;
    cout << "Ending application - destructors called" << endl;

    return 0;
}