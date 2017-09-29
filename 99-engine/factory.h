#pragma once

#include <functional>
#include <unordered_map>
#include <exception>

template<typename Product, typename Identifier, typename... Args>
class factory
{
public:
    using FunType = std::function<Product(Args...)>;
private:
    using store_t = std::unordered_map<Identifier, FunType>;
    store_t _constructors;

protected:
    factory() = default;

public:

    void register_constructor(const Identifier &id, FunType constructor)
    {
        _constructors[id] = constructor;
    }

    Product create(const Identifier &id, Args... args)
    {
        auto fun = _constructors.find(id);
        if (fun != _constructors.end())
        {
            return fun->second(args...);
        }
        throw std::invalid_argument("Error creating object");
    }
};