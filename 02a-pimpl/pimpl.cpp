#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// object contains a bunch of data.  We want to avoid copying, but we want to avoid pointers in 
// top level code.  Use Private IMPLementation, or Pointer to IMPLementation (PIMPL) idiom.
class object
{
private:
    struct object_impl
    {
        vector<float> data;

        // Constructor allocates 2^24 floats or 64MB of data.
        object_impl()
        : data(1 << 24)
        {
            cout << "Size of data = " << data.size() << endl;
        }
    };

    // Use either shared pointer or unique pointer depending on what you are doing.
    // Shared pointer if multiple references of object_impl is used.
    // Unique pointer if only one instance exists.
    shared_ptr<object_impl> _self = nullptr;

public:
    // Job of constructor is to initialise the _self pointer.
    object()
    : _self{make_shared<object_impl>()}
    {
    }

    // We can now access data from our object
    // Note use of const (not changing object)
    // Note use of noexcept (function cannot throw exception)
    // This can help the compiler.
    size_t check_size() const noexcept { return _self->data.size(); }

    // Add a value to the vector
    void add_value(float x) { _self->data.push_back(x); }

    // Gets the last value in the data
    float get_last() const noexcept { return _self->data[_self->data.size() - 1]; }
};

// Test function
void do_test(object obj)
{
    cout << "Size of data in do_test = " << obj.check_size() << endl;
    cout << "Value at end of data in do_test = " << obj.get_last() << endl;
}

int main(int argc, char **argv)
{
    object obj;
    cout << "Size of data at start = " << obj.check_size() << endl;
    obj.add_value(5.0f);
    cout << "Size of data after add = " << obj.check_size() << endl;
    cout << "Value at end of data = " << obj.get_last() << endl;
    cout << "Calling do_test function" << endl;
    do_test(obj);
    return 0;
}