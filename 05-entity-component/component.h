#pragma once

#include <memory>

// Forward declaration
class entity;

class component
{
protected:
    // Flag to indicate the component is active (should be updated)
    bool _active = true;

    // Flag to indicate the component is visible (should be rendered)
    bool _visible = true;

    // Use PIMPL.
    struct component_impl
    {
        // Pointer to the parent
        entity &_parent;

        component_impl(entity &parent) : _parent(parent) { }
        virtual ~component_impl() = default;
        virtual bool initialise() = 0;
        virtual void update(float delta_time) = 0;
        virtual void render() = 0;
        virtual void shutdown() = 0;
    };

    std::shared_ptr<component_impl> _self = nullptr;

    component(std::shared_ptr<component_impl> self, bool active, bool visible)
    : _self{self}, _active(active), _visible(visible)
    {
    }

public:

    component() { } 

    virtual ~component() = default;

    bool get_active() const noexcept { return _active; }

    void set_active(bool value) noexcept { _active = value; }

    bool get_visible() const noexcept { return _visible; }

    void set_visible(bool value) noexcept { _visible = value; }

    bool initialise()
    {
        return _self->initialise();
    }

    void update(float delta_time)
    {
        if (_active)
            _self->update(delta_time);
    }

    void render()
    {
        if (_visible)
            _self->render();
    }

    void shutdown()
    {
        _self->shutdown();
    }
};