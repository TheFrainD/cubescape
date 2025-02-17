#pragma once

namespace core {
union Dimensions {
    struct {
        int width;
        int height;
    };
    struct {
        int x;
        int y;
    };

    bool operator==(const Dimensions& other) const {
        return width == other.width && height == other.height;
    }

    bool operator!=(const Dimensions& other) const { return !(*this == other); }
};
}  // namespace core