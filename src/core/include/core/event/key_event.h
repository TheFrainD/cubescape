#pragma once

#include "core/input/key.h"

namespace core {
struct KeyPressedEvent {
    Key key;
};

struct KeyReleasedEvent {
    Key key;
};
}  // namespace core
