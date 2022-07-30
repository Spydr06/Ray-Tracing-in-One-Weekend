#pragma once

#include "common.hpp"
#include "color.hpp"
#include "perlin.hpp"

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

class NoiseTexture : public Texture {
    public:
        NoiseTexture() {}
        NoiseTexture(double sc) : scale(sc) {}

        virtual Color value(double u, double v, const Point3 &p) const override 
        { 
            return Color(1,1,1) * 0.5 * (1 + sin(scale*p.z() + 10*noise.turb(p))); 
        }
    
    public:
        Perlin noise;
        double scale;
};

class ImageTexture : public Texture {
    public:
        const static int bytes_per_pixel = 0x03;

        ImageTexture()
            : data(nullptr), width(0), height(0), bytes_per_scanline(0)
        {}

        ImageTexture(const char* filename);
        ~ImageTexture() { delete data; }

        virtual Color value(double u, double v, const Vec3 &p) const override;

    private:
        unsigned char *data;
        int width, height;
        int bytes_per_scanline;
};

} // namespace raytracing