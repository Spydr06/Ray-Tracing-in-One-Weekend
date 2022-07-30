#pragma once

#include "common.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "vec3.hpp"
#include <memory>

namespace raytracing {

struct HitRecord;

class Material {
    public:
        virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const = 0;
        virtual Color emitted(double u, double v, const Point3 &p) const { return Color(0,0,0); }
};

class Lambertian: public Material {
    public:
        Lambertian(const Color &a) : albedo(std::make_shared<SolidColor>(a)) {}
        Lambertian(std::shared_ptr<Texture> a) : albedo(a) {}

        virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;

    public:
        std::shared_ptr<Texture> albedo;
};

class Metal : public Material {
    public:
        Metal(const Color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;

    public:
        Color albedo;
        double fuzz;
};

class Dielectric : public Material {
    public:
        Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;

    public:
        double ir;

    private:
        static double reflectance(double cosine, double ref_idx);
};

class DiffuseLight : public Material {
    public:
        DiffuseLight(std::shared_ptr<Texture> a) : emit(a) {}
        DiffuseLight(Color c) : emit(std::make_shared<SolidColor>(c)) {}

        virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
        {
            return false;
        }

        virtual Color emitted(double u, double v, const Point3 &p) const override {
            return emit->value(u, v, p);
        }

    public:
        std::shared_ptr<Texture> emit;
};

} // namespace raytracing