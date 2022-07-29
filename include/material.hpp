#pragma once

#include "common.hpp"
#include "ray.hpp"

namespace raytracing {

struct HitRecord;

class Material {
    public:
        virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const = 0;
};

class Lambertian: public Material {
    public:
        Lambertian(const Color &a) : albedo(a) {}

        virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;

    public:
        Color albedo;
};

class Metal : public Material {
    public:
        Metal(const Color &a) : albedo(a) {}

        virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;

    public:
        Color albedo;
};

} // namespace raytracing