#pragma once

#include "common.hpp"
#include "hittable.hpp"

#include <iostream>
#include <ostream>

namespace raytracing {

class BVHNode : public Hittable {
    public:
        BVHNode();
        BVHNode(const HittableList &list, double time0, double time1)
            : BVHNode(list.objects, 0, list.objects.size(), time0, time1)
        {}

        BVHNode(const std::vector<std::shared_ptr<Hittable>> &src_objects, size_t start, size_t end, double time0, double time1);

        virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
        virtual bool bounding_box(double time0, double time1, AABB &output_box) const override;

    public:
        std::shared_ptr<Hittable> left;
        std::shared_ptr<Hittable> right;
        AABB box;
};

inline bool box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis)
{
    AABB box_a, box_b;

    if(!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in BVHNode constructor." << std::endl;
    
    return box_a.min().e[axis] < box_b.min().e[axis];
}

} // namespace raytracing