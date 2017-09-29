#pragma once

#include <unordered_map>
#include <functional>

template<typename Base, typename KeyType, typename... Args>
class generic_factory
{
public:
    using FunType = std::function<Base(Args...)>;
private:
    using FnMap = std::unordered_map<KeyType, FunType>;
    FnMap FnList;
public:
    generic_factory() = default;
    generic_factory(const generic_factory&) = delete;
    generic_factory& operator=(const generic_factory &) = delete;

    void reg(const KeyType &key, FunType fn)
    {
        FnList[key] = fn;
    }

    Base create(const KeyType &key, Args... args)
    {
        auto found = FnList.find(key);
        return found->second(std::forward<Args...>(args...));
    }
};