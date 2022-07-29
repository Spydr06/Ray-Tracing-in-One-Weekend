#pragma once

#include "ray.hpp"

#include <memory>
#include <vector>

namespace raytracing {

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t;
    bool front_face;

    inline void set_face_normal(const Ray &r, const Vec3 &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord &rec) const = 0;
};

class HittableList : public Hittable
{
    public:
        HittableList() {}
        HittableList(std::shared_ptr<Hittable> object) { add(object); }

        virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

        void clear() { objects.clear(); }
        void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    public:
        std::vector<std::shared_ptr<Hittable>> objects;
};

} // namespace raytracing