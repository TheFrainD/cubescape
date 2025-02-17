#pragma once

#include "core/input/key.h"
#include "core/input/mouse_button.h"
#include "core/math/direction.h"
#include "core/math/point.h"

namespace core {
class InputManager {
public:
    InputManager() = delete;

    /**
     * @brief Initializes the input manager.
     *
     * @param window The window to initialize the input manager with.
     */
    static void Init();

    /**
     * @brief Checks if a key is pressed.
     *
     * @param key The key to check.
     * @return true If the key is pressed.
     */
    static bool KeyPressed(Key key);

    /**
     * @brief Checks if a mouse button is pressed.
     *
     * @param button The mouse button to check.
     * @return true If the mouse button is pressed.
     */
    static bool MouseButtonPressed(MouseButton button);

    /**
     * @brief Retrieves the position of the mouse.
     *
     * @return math::Point2D The position of the mouse.
     */
    static math::Point2D GetMousePosition();

    /**
     * @brief Retrieves the mouse delta.
     *
     * @return math::Direction2D The mouse delta.
     */
    static math::Direction2D GetMouseDelta();

private:
    static math::Point2D mouse_prev_position_;
    static math::Direction2D mouse_delta_;
};
}  // namespace core