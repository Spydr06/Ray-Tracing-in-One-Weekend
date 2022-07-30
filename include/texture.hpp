#pragma once

#include "common.hpp"
#include "color.hpp"
#include <memory>

namespace raytracing {

class Texture {
    public:
        virtual Color value(double u, double v, const Point3 &p) const = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor() {}
        
        SolidColor(Color c) : color_value(c) {}

        SolidColor(double red, double green, double blue)
            : SolidColor(Color(red, green, blue)) 
        {}

        virtual Color value(double u, double v, const Point3 &p) const override { return color_value; }
        
    private:
        Color color_value;
};

class CheckerTexture : public Texture {
    public:
        CheckerTexture() {}
        
        CheckerTexture(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd)
            : even(_even), odd(_odd) 
        {}
    
        CheckerTexture(Color c1, Color c2)
            : even(std::make_shared<SolidColor>(c1)), odd(std::make_shared<SolidColor>(c2))
        {}

        virtual Color value(double u, double v, const Point3 &p) const override;

    public:
        std::shared_ptr<Texture> odd;
        std::shared_ptr<Texture> even;
};

} // namespace raytracing