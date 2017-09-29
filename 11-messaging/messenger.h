#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <functional>

// We will use the mediator pattern to allow decoupled communication.
// We will combine this with the command pattern.
template<typename T>
class command
{
private:
    std::function<void(T&)> _fun;
public:
    command() = default;

    command(std::function<void(T&)> fun)
    : _fun(fun)
    {
    }

    void execute(T &obj)
    {
        _fun(obj);
    }
};

template<typename T>
class message_handler
{
private:
    // Vector of commands.
    std::vector<command<T>> _messages;
public:
    message_handler()
    : _messages(32)
    {
    }

    void handle_message(command<T> &message)
    {
        _messages.push_back(message);
    }

    void execute(T &obj)
    {
        for (auto &m : _messages)
            m.execute(obj);
        _messages.clear();
    }
};

template<typename T>
class mediator
{
private:
    std::unordered_map<std::string, message_handler<T>> _handlers;
public:
    void send_message(const std::string &destination, command<T> message)
    {
        auto found = _handlers.find(destination);
        if (found != _handlers.end())
        {
            found->second.handle_message(message);
        }
    }

    void add_handler(const std::string &name, message_handler<T> handler)
    {
        _handlers[name] = handler;
    }
};