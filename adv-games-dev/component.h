#pragma once

#include <memory>

class component
{
private:
    struct concept_t
    {
        virtual ~concept_t() = default;

        virtual bool initialise() = 0;
        virtual bool load_content() = 0;
        virtual void update(float delta_time) = 0;
        virtual void render() = 0;
        virtual void unload_content() = 0;
        virtual void shutdown() = 0;
    };

    template<typename T>
    struct model_t : concept_t
    {
        T _self;

        model_t(T s) : _self{std::move(s)} {}
        bool initialise() override { return _self.initialise(); }
        bool load_content() override { return _self.load_content(); }
        void update(float delta_time) override { _self.update(delta_time); }
        void render() override { _self.render(); }
        void unload_content() override { _self.unload_content(); }
        void shutdown() override { _self.shutdown(); }
    };

    std::unique_ptr<concept_t> _self = nullptr;

    bool _active = true;
    bool _visible = true;

public:
    component() = default;

    template<typename T>
    component(T t, bool active = true, bool visible = true) : _self{std::make_unique<model_t<T>>(std::move(t))}, _active(active), _visible(visible) 
	{
	
	}

    void set_active(bool value) noexcept { _active = value; }
    void set_visible(bool value) noexcept { _visible = value; }
    bool get_active() const noexcept { return _active; }
    bool get_visible() const noexcept { return _visible; }

    bool initialise()
    {
        return _self->initialise();
    }

    bool load_content()
    {
        return _self->load_content();
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

    void unload_content()
    {
        _self->unload_content();
    }

    void shutdown()
    {
        _self->shutdown();
    }

    template<typename T>
    T& get() 
    {
        return (dynamic_cast<model_t<T>*>(_self.get()))->_self;
    }
};