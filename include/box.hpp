#pragma once

#include "common.hpp"
#include "aarect.hpp"
#include "hittable.hpp"
#include "vec3.hpp"
#include <memory>

namespace raytracing {

class Box : public Hittable {
    public:
        Box() {}
        Box(const Point3 &p0, const Point3 &p1, std::shared_ptr<Material> ptr);
    
        virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
        virtual bool bounding_box(double time0, double time1, AABB &output_box) const override
        {
            output_box = AABB(box_min, box_max);
            return true;
        }
    
    public:
        Point3 box_min, box_max;
        HittableList sides;
};

} // namespace raytracing