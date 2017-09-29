#include <iostream>

// It is best to tie resources to the object that owns them
// When the object dies so does the resource.
// See smart pointers, lock guards, etc.
// This gives you control over resource allocation and 
// deallocation.

class my_data
{
private:
    float *_data = 0;
public:
    my_data(float *data) : _data(data) { }

    ~my_data() { delete[] _data; }
};

int main(int argc, char **argv)
{
    {
        // Allocate resource with object creation
        my_data obj(new float[1000]);
    }
    // obj is out of scope, and the resource will be freed.

    return 0;
}