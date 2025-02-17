#pragma once

#include "core/event/event_bus.h"

namespace core {
struct Context {
    EventBus event_bus;
};
}  // namespace core