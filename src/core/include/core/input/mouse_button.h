#pragma once

namespace core {
enum class MouseButton : int {
    // From glfw3.h
    kButton0 = 0,
    kButton1 = 1,
    kButton2 = 2,
    kButton3 = 3,
    kButton4 = 4,
    kButton5 = 5,
    kButton6 = 6,
    kButton7 = 7,

    kButtonLast   = kButton7,
    kButtonLeft   = kButton0,
    kButtonRight  = kButton1,
    kButtonMiddle = kButton2,

    kLast = kButton7
};
}
