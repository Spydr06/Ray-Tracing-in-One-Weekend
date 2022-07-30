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
        virtual bool bounding_box(double time0, double time1, AABB &output_box) const override;

    private:
        static void get_sphere_uv(const Point3 &p, double &u, double &v);

    public:
        Point3 center;
        double radius;
        std::shared_ptr<Material> mat_ptr;
};

class MovingSphere : public Hittable {
    public:
        MovingSphere() {}
        MovingSphere(Point3 cen0, Point3 cen1, double _time0, double _time1, double r, std::shared_ptr<Material> m)
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
        {}

        virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
        virtual bool bounding_box(double time0, double time1, AABB &output_box) const override;

        Point3 center(double time) const;

    public:
        Point3 center0, center1;
        double time0, time1, radius;
        std::shared_ptr<Material> mat_ptr;
};

} // namespace raytracing