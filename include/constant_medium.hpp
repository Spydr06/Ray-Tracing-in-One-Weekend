#pragma once

#include "common.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "texture.hpp"
#include <memory>

namespace raytracing {

class ConstantMedium : public Hittable {
    public:
        ConstantMedium(std::shared_ptr<Hittable> b, double d, std::shared_ptr<Texture> a)
            : boundary(b), neg_inv_density(-1 / d), phase_function(std::make_shared<Isotropic>(a))
        {}

        ConstantMedium(std::shared_ptr<Hittable> b, double d, Color c)
            : boundary(b), neg_inv_density(-1 / d), phase_function(std::make_shared<Isotropic>(c))
        {}

        virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
        virtual bool bounding_box(double time0, double time1, AABB &output_box) const override
        { 
            return boundary->bounding_box(time0, time1, output_box); 
        }

    public:
        std::shared_ptr<Hittable> boundary;
        double neg_inv_density;
        std::shared_ptr<Material> phase_function;
};

} // namespace raytracing