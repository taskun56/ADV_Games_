#pragma once

#include <memory>

// Represents a subsystem of the engine.  For example, physics, rendering, AI.
// What you define as a system is your choice.
class subsystem
{
private:
    // concept_t provides the interface to our private subsystem component.
    // All methods are virtual.
    struct concept_t
    {
        // We need a virtual destructor as we will be inheriting from this object.
        // ALWAYS define a deconstructor.  Even if you can default it at the start.
        // ALWAYS make your deconstructor virtual if you plan inheritance from this class.
        virtual ~concept_t() = default;

        // The following methods define the methods that will be provided to the engine
        // by a subsystem.  All are pure virtual.

        // Called when the engine is initialised.
        virtual bool initialise() = 0;

        // Called when all the content should be loaded.
        virtual bool load_content() = 0;

        // Called when the engine is updated.
        virtual void update(float delta_time) = 0;

        // Called when the engine is rendered.
        virtual void render() = 0;

        // Called when all the content is unloaded.
        virtual void unload_content() = 0;

        // Called when the engine is shutdown.
        virtual void shutdown() = 0;
    };

    // model_t is the concrete implementation of the concept.  It is like the adapter
    // pattern.  It wraps around an object and allows calling of the interfaced methods
    // as required.
    // *** SEE DESCRIPTION AFTER DECLARATION TO UNDERSTAND HOW IT WORKS ***
    template<typename T>
    struct model_t : concept_t
    {
        // The wrapped object.
        T _self;

        // Creates an instance of model_t.  Simply wraps the provided object.
        // Uses move semantics to avoid copying the object.
        model_t(T s) : _self{std::move(s)} {}

        // Initialise will call initialise on the wrapped object.
        // Note use of override as we are overriding the pure virtual function of concept_t.
        bool initialise() override { return _self.initialise(); }

        // load_content will call load_content on the wrapped object.
        // Note use of override as we are overriding the pure virtual function of concept_t
        bool load_content() override { return _self.load_content(); }

        // Update will call update on the wrapped object.
        // Note use of override as we are overriding the pure virtual function of concept_t.
        void update(float delta_time) override { _self.update(delta_time); }

        // Render will call render on the wrapped object.
        // Note use of override as we are overriding the pure virtual function of concept_t.
        void render() override { _self.render(); }

        // unload_content will call unload_content on the wrapped object.
        // Note use of override as we are overriding the pure virtual function of concept_t.
        void unload_content() override { _self.unload_content(); }

        // Shutdown will call shutdown on the wrapped object.
        // Note use of override as we are overriding the pure virtual function of concept_t.
        void shutdown() override { return _self.shutdown(); }
    };
    // This works due to templating "magic".  model_t is not defined until we provide
    // a particular type.  Once defined, all that matters is that T has methods initialise
    // update, render, and shutdown.  If it doesn't, we get a compiler error.  No
    // inheritance of T from concept_t is required.

    // Flag to indicate that the system is currently active and should be updated
    // each frame.
    bool _active = true;

    // Flag to indicate that the system has rendererable components and should have
    // its components rendered per frame.
    bool _visible = true;

    // Pointer to the internal object representing the subsystem.
    // We are using PIMPL here:
    // ** Pointer to IMPLementation, or
    // ** Private IMPLementation
    // This will be initialised when an instance of the object is created.
    // Allows simple C++ polymorphism and light object copy (single unique_ptr copy
    // is 4-8 bytes).
    // As the pointer will not be shared outside the class use unique_ptr.
    // Will automatically be recovered when out of scope.
    std::unique_ptr<concept_t> _self = nullptr;

public:
    // Default constructor.  Only required as we are holding systems in a data
    // structure.  Unused otherwise.
    subsystem() = default;

    // The constructor that is called.  This is templated, and will accept
    // an object that needs to be wrapped in an instance of model_t.  Note the use
    // of default parameter values.
    template<typename T>
    subsystem(T t, bool active = true, bool visible = true)
    : _self{std::make_unique<model_t<T>>(std::move(t))}, _active(active), _visible(visible)
    {
    }

    // Define default copy constructor
    subsystem(const subsystem &other) = default;

    // Define default move constructor
    subsystem(subsystem &&rhs) = default;

    // Define default copy assignment operator.
    subsystem& operator=(const subsystem &other) = default;

    // Define default move assignment operator.
    subsystem& operator=(subsystem &&rhs) = default;

    // Destructor.  Not virtual.  You should not inherit from this class.
    ~subsystem() = default;

    // Gets the current active status.
    // Note a getter is const (does not change the object) and does not throw an exception
    // noexcept
    bool get_active() const noexcept { return _active; }

    // Gets the current visible status.
    // Note a getter is const (does not change the object) and does not throw an exception
    // noexcept
    bool get_visible() const noexcept { return _visible; }

    // Sets the value of active.
    // Note the setter does not throw an exception - noexcept.
    void set_active(bool active) noexcept { _active = active; }

    // Sets the value of visible.
    // Note the setter does not throw an exception - noexcept.
    void set_visible(bool visible) noexcept { _visible = visible; }

    // This is the publically visible initialise method.  Will call the wrapped
    // object internally.
    bool initialise()
    {
        return _self->initialise();
    }

    // This is the publically visible load_content method.  Will call the wrapped
    // object internally.
    bool load_content()
    {
        return _self->load_content();
    }

    // This is the publically visible update method.  Will call the wrapped object
    // internally.
    void update(float delta_time)
    {
        // Check if update should be called.
        if (_active)
            _self->update(delta_time);
    }

    // This is the publically visible render method.  Will call the wrapped object
    // internally.
    void render()
    {
        // Check if render should be called
        if (_visible)
            _self->render();
    }

    // This is the publically visible unload_content method.  Will call the wrappe object
    // internally.
    void unload_content()
    {
        _self->unload_content();
    }

    // This is the publically visible shutdown method.  Will call the wrapped object
    // internally.
    void shutdown()
    {
        _self->shutdown();
    }

    // Gets the internally wrapped object.  This is some trickery with templating
    // and dereferencing.  This is used by engine.
    template<typename T>
    T& get()
    {
        // I've broken this down into seperate lines although it should only be a
        // single line.
        // First get the pointed to concept_t.
        auto ptr = _self.get();
        // Now cast to model_t of type T.  Use dynamic cast.  If it fails this will
        // be a nullptr and crash your program.
        auto ptr_model = dynamic_cast<model_t<T>*>(ptr);
        // We then just return the self value in ptr_model.
        return ptr_model->_self;
        // This is returned as a reference.  No copying occurs.
    }
};