#pragma once

#include <unordered_map>
#include <functional>
#include <exception>

template<typename T>
class state_machine
{
private:
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

    struct state_machine_impl
    {
        std::string _current_state = "";
        state_details _current_state_functions;
    
        std::unordered_map<std::string, state_details> _states;
    };

    std::shared_ptr<state_machine_impl> _self = nullptr;
public:
    state_machine()
    : _self{new state_machine_impl()}
    {
    }

    void add_state(const std::string &name, std::function<void(T&)> enter, std::function<void(T&)> update, std::function<void(T&)> exit)
    {
        state_details details(enter, update, exit);
        _self->_states[name] = details;
    }

    void change_state(const std::string &name, T& owner)
    {
        if (name != _self->_current_state)
        {
            auto found = _self->_states.find(name);
            if (found != _self->_states.end())
            {
                if (_self->_current_state != "")
                    _self->_current_state_functions._exit(owner);
                _self->_current_state_functions = found->second;
                _self->_current_state_functions._enter(owner);
                _self->_current_state = name;
            }
            else
            {
                throw std::invalid_argument("state not found");
            }
        }
    }

    void update(T& owner)
    {
        _self->_current_state_functions._update(owner);
    }
};