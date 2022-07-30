#pragma once

#include "common.hpp"
#include "hittable.hpp"

namespace raytracing {

class XYRect : public Hittable {
    public:
        XYRect() {};
        XYRect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<Material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat)
        {}

        virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

        virtual bool bounding_box(double time0, double time1, AABB &output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = AABB(Point3(x0, y0, k - 0.0001), Point3(x1, y1, k + 0.0001));
            return true;
        }


    public:
        std::shared_ptr<Material> mp;
        double x0, x1, y0, y1, k;
};

class XZRect : public Hittable {
    public:
        XZRect() {};
        XZRect(double _x0, double _x1, double _z0, double _z1, double _k, std::shared_ptr<Material> mat)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat)
        {}

        virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

        virtual bool bounding_box(double time0, double time1, AABB &output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = AABB(Point3(x0, k - 0.0001, z0), Point3(x1, k + 0.0001, z1));
            return true;
        }


    public:
        std::shared_ptr<Material> mp;
        double x0, x1, z0, z1, k;
};

class YZRect : public Hittable {
    public:
        YZRect() {};
        YZRect(double _y0, double _y1, double _z0, double _z1, double _k, std::shared_ptr<Material> mat)
            : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat)
        {}

        virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

        virtual bool bounding_box(double time0, double time1, AABB &output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = AABB(Point3(k - 0.0001, y0, z0), Point3(k + 0.0001, y1, z1));
            return true;
        }


    public:
        std::shared_ptr<Material> mp;
        double y0, y1, z0, z1, k;
};

} // namespace raytracing