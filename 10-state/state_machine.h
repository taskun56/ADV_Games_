#pragma once

#include <unordered_map>
#include <functional>
#include <exception>

// A state machine decides which states to switch into.
// We will just use strings as identifiers, but numbers
// or enums are far more efficient.
// If your states are purely code, then you could use
// static methods and make things more efficient.  Depends
// on your level of abstraction.
template<typename T>
class state_machine
{
private:
    // This structure contains the three parts of state
    // transition.
    // * enter - called when the state is first entered.
    // * update - called each frame.
    // * exit - called when the state is exited.
    struct state_details
    {
        std::function<void(T&)> _enter;
        std::function<void(T&)> _update;
        std::function<void(T&)> _exit;

        state_details() = default;

        state_details(std::function<void(T&)> enter, std::function<void(T&)> update, std::function<void(T&)> exit)
        : _enter(enter), _update(update), _exit(exit)
        {
        }
    };

    std::string _current_state = "";
    state_details _current_state_functions;

    std::unordered_map<std::string, state_details> _states;
public:
    state_machine() = default;

    void add_state(const std::string &name, std::function<void(T&)> enter, std::function<void(T&)> update, std::function<void(T&)> exit)
    {
        state_details details(enter, update, exit);
        _states[name] = details;
    }

    void change_state(const std::string &name, T& owner)
    {
        if (name != _current_state)
        {
            auto found = _states.find(name);
            if (found != _states.end())
            {
                if (_current_state != "")
                    _current_state_functions._exit(owner);
                _current_state_functions = found->second;
                _current_state_functions._enter(owner);
                _current_state = name;
            }
            else
            {
                throw std::invalid_argument("state not found");
            }
        }
    }

    void update(T& owner)
    {
        _current_state_functions._update(owner);
    }
};