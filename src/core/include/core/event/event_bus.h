#pragma once

#include <entt/entt.hpp>

namespace core {
class EventBus {
public:
    template <typename Event, typename... Args>
    void Trigger(Args &&...args) {
        dispatcher_.trigger<Event>(std::forward<Args>(args)...);
    }

    template <typename Event>
    void Trigger(const Event &event) {
        dispatcher_.trigger(event);
    }

    template <typename Event, auto Method, typename Class>
    void Connect(Class &instance) {
        dispatcher_.sink<Event>().connect<Method>(&instance);
    }

    template <typename Event, auto Function>
    void Connect() {
        dispatcher_.sink<Event>().connect<Function>();
    }

private:
    entt::dispatcher dispatcher_;
};
}  // namespace core
