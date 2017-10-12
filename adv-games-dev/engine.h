#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <exception>
#include "subsystem.h"
#include "singleton.h"
#include "state_machine.h"

class engine : public singleton<engine>, public state_machine<engine>
{
    friend class singleton<engine>;
private:
    // engine_impl is the private implementation of engine.  Ensures that we only
    // copy a pointer when we create a new reference to the engine.  The engine will
    // be referenced by each subsystem.
    struct engine_impl
    {
        // The collection of subsystems.  Note the type <type_index, subsystem>.  type_index
        // is a unique identifier for a type.  THIS IS NOT NECESSARILY EFFICIENT.  If you
        // are interested look up RTTI (Run Time Type Information) in C++.  I use it here
        // to make life easier.
        // A BETTER method would be to define your own indexing or to use strings.  This
        // would allow multiple subsystems of the same type.
        std::unordered_map<std::type_index, subsystem> _subsystems;
    };

    // Pointer to the implementation.  This is shared as each subsystem will have a
    // reference to the engine.
    std::shared_ptr<engine_impl> _self = nullptr;

    // Flag to indicate if the engine is running or not.
    bool _running = true;

    // Private constructor.  Called when we call get.
    engine()
    : _self(new engine_impl())
    {
    }

public:

    // Adds a subsystem to the engine.
    template<typename T>
    void add_subsystem(T sys, bool active = true, bool visible = true)
    {
        // We aren't doing any checking here.  We are assuming that no other subsystem
        // of this type exists.  If it does, it will be overriden.
        // We use curly braces construction which is the currently advised method.
        // The curly braces could be replaced by normal parenthesis.
        _self->_subsystems[std::type_index(typeid(T))] = subsystem{sys, active, visible};
    }

    // Gets a subsystem from the engine.  We use the type to look up and then do
    // some template magic to convert it to the type we want by getting the internally
    // wrapped object.  We also return a reference to avoid copying.
    template<typename T>
    T& get_subsystem()
    {
        // This could be done in compressed lines.  I will do this a line at a time for
        // illustration purposes.
        // Find the system of the given type.
        auto found = _self->_subsystems.find(std::type_index(typeid(T)));
        // Found is either a valid value or the end of the collection.  Deal with the
        // latter.
        if (found == _self->_subsystems.end())
        {
            // No subsystem of type found.  Fail.
            throw std::invalid_argument("Failed to find subsystem");
        }
        // We know a subsystem of the type was found.  We need to return it as the
        // correct type.  Thankfully subsystem has code to help us.
        return found->second.get<T>();
    }

    // You might want to add code for removing subsystems.

    // Get the current running value.
    bool get_running() const noexcept { return _running; }

    // Set the current running value.
    void set_running(bool value) noexcept { _running = value; }

    // Runs the engine.  Note that this technique takes no account of subsystem order.
    // If subsystem order is important consider using another mechanism.
    void run()
    {
        // Initialise all the subsystems
        for (auto &sys : _self->_subsystems)
        {
            // If initialise fails exit run.
            if (!sys.second.initialise())
            {
                return;
            }
        }

        // Load content for all the subsystems
        for (auto &sys : _self->_subsystems)
        {
            // If load_content fails exit run.
            if (!sys.second.load_content())
            {
                return;
            }
        }

        // Loop until not running.
        while (_running)
        {
            std::cout << "Engine Running" << std::endl;
            // Update the subsystems.  At the moment use dummy time of 1.0s.  You
            // will want to use a proper timer.
            for (auto &sys : _self->_subsystems)
                sys.second.update(1.0);
            // Render the subsystems.
            for (auto &sys : _self->_subsystems)
                sys.second.render();
        }

        // Unload the content.
        for (auto &sys : _self->_subsystems)
        {
            sys.second.unload_content();
        }

        // Shutdown subsystems
        for (auto &sys : _self->_subsystems)
        {
            sys.second.shutdown();
        }
        // Clear out all the subsystems causing destructors to call.
        _self->_subsystems.clear();
        // Engine will now exit.
    }
};