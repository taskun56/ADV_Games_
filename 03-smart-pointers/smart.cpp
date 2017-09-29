#include <iostream>
#include <memory>

using namespace std;

// If you simply must use pointers in your top level
// code, use smart ones.

void test_shared(shared_ptr<int> shared)
{
    cout << "Use count in test_shared = " << shared.use_count() << endl;
}

void test_unique(unique_ptr<int> unique)
{
    cout << "Address pointed to in test_unique = " << unique.get() << endl;
}

// Use a weak pointer for none owning references.
weak_ptr<int> weak;

void test_weak()
{
    // Gets the number of references
    cout << "use count = " << weak.use_count() << ": ";
    // We have to convert the weak pointer to a shared pointer
    if (auto shared = weak.lock())
    {
        cout << *shared << endl;
    }
    else
    {
        cout << "weak has expired" << endl;
    }
}

int main(int argc, char **argv)
{
    // Use make_shared to create a shared pointer
    auto data = make_shared<int>(42);
    cout << "Usage count of data = " << data.use_count() << endl;
    test_shared(data);
    cout << "Usage count of data after test_shared = " << data.use_count() << endl;

    // Use make_unique to create a unique pointer
    auto unique = make_unique<int>(42);
    cout << "Address pointed to by unique pointer = " << unique.get() << endl;
    // A unique pointer needs to be moved.  Copy constructor is deleted.
    test_unique(std::move(unique));
    cout << "Address pointed to after test_unique = " << unique.get() << endl;

    {
        // Make a shared pointer
        auto shared = make_shared<int>(42);
        // Grab a weak reference
        weak = shared;
        // Call test_weak
        test_weak();
    }
    // Shared is now out of scope
    test_weak();

    return 0;
}