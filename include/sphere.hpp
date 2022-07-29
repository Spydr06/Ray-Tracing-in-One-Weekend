#pragma once

#include "common.hpp"
#include "hittable.hpp"

namespace raytracing {

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Point3 cen, double r, std::shared_ptr<Material> m) 
            : center(cen), radius(r), mat_ptr(m)
        {}

        virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

    public:
        Point3 center;
        double radius;
        std::shared_ptr<Material> mat_ptr;
};


} // namespace raytracing