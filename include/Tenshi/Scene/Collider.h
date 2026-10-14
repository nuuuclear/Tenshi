#pragma once

#include "Tenshi/Geometry/Rect.h"

namespace Tenshi {

class Collider {
public:
    double offsetX = 0;
    double offsetY = 0;

    double w = 0;
    double h = 0;

    Rect<double> getBounds(double entityX, double entityY) const {
        return {
            entityX + offsetX,
            entityY + offsetY,
            w,
            h
        };
    }

    static bool intersects(const Rect<double>& a, const Rect<double>& b) {
        if (a.x + a.w <= b.x) return false;
        if (b.x + b.w <= a.x) return false;
        if (a.y + a.h <= b.y) return false;
        if (b.y + b.h <= a.y) return false;
        return true;
    }
};

} // namespace Tenshi