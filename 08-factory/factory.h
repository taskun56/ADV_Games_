#pragma once

#include <functional>
#include <unordered_map>
#include <exception>

// Templating again gives us the ability to simplify factory
// definitions.

// Product is the type that is created.
// Identifier is a lookup key for construction functions.
template<typename Product, typename Identifier>
class factory
{
private:
    // A map of identifier to a builder function.
    // We give a short name for the type to make our life easier.
    typedef std::unordered_map<Identifier, std::function<Product()>> store_t;
    store_t _constructors;

public:
    void register_constructor(const Identifier &id, std::function<Product()> constructor)
    {
        _constructors[id] = constructor;
    }

    Product create(const Identifier &id)
    {
        auto fun = _constructors.find(id);
        if (fun != _constructors.end())
        {
            return fun->second();
        }
        throw std::invalid_argument("Error creating object");
    }
};