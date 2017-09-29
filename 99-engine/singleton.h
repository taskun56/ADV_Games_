#pragma once

#include <memory>

template<typename T>
class singleton
{
public:
    // This call needs to be inline and static
    inline static T& get()
    {
        static std::unique_ptr<T> instance(new T());
        // Dereference the pointer to get the data.
        return *instance.get();
    }
};