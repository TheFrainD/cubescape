#pragma once

#include "core/input/mouse_button.h"

#include "core/math/point.h"
#include "core/math/direction.h"

namespace core {
struct MouseButtonPressedEvent {
    MouseButton button;
};

struct MouseButtonReleasedEvent {
    MouseButton button;
};

struct MouseMovedEvent {
    math::Point2D position;
    math::Direction2D delta;
};
}  // namespace core
