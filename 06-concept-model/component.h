#pragma once

#include <memory>

// The concept-model pattern is an extension to PIMPL
// and the adapter pattern.  It uses template "magic" to
// get around some of the complexities of simple PIMPL.
// Basically, we don't need to worry about inheritance.
class component
{
private:
    // We keep a pointer to a concept.  This looks a lot like
    // PIMPL at this point, but we are going to hide it.
    struct concept_t
    {
        virtual ~concept_t() = default;

        virtual bool initialise() = 0;
        virtual void update(float delta_time) = 0;
        virtual void render() = 0;
        virtual void shutdown() = 0;
    };

    // The model type wraps an object of type T.
    // T can be anything as long as it implements the methods
    // initialise, update, render, and shutdown.  The 
    // pre-processor replaces the types, meaning inheritance
    // has nothing to do with what we are doing.
    template<typename T>
    struct model_t : concept_t
    {
        T _self;

        // We just need an object to wrap.
        model_t(T c) : _self{std::move(c)} {}

        bool initialise() override { return _self.initialise(); }
        void update(float delta_time) override { _self.update(delta_time); }
        void render() override { _self.render(); }
        void shutdown() override { _self.shutdown(); }
    };

    std::shared_ptr<concept_t> _self = nullptr;

    bool _active = true;

    bool _visible = true;

public:
    // We need an empty constructor for various structures.
    component() = default;

    // We don't know what type we are going to wrap, so we
    // have to use templating.
    template<typename T>
    component(T t, bool active = true, bool visible = true)
    : _self{std::make_shared<model_t<T>>(std::move(t))}, _active(active), _visible(visible)
    {
    }

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

    // We need this to get the wrapped object.
    template<typename T>
    const T& get() const
    {
        return (dynamic_cast<model_t<T>*>(_self.get()))->self;
    }
};