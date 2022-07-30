#pragma once

#include "vec3.hpp"
#include "ray.hpp"

namespace raytracing {

class AABB {
    public:
        AABB() {}
        AABB(const Point3 &a, const Point3 &b)
            : maximum(a), minimum(b)
        {}

        Point3 min() const { return minimum; }
        Point3 max() const { return maximum; }

        bool hit(const Ray &r, double t_min, double t_max) const;

    public:
        Point3 maximum, minimum;
};

AABB surrounding_box(AABB box0, AABB box1);

} // namespace raytracing