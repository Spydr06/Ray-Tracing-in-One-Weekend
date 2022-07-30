#include <texture.hpp>

namespace raytracing {
    Color CheckerTexture::value(double u, double v, const Point3 &p) const
    {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        return sines < 0 ? odd->value(u, v, p) : even->value(u, v, p);
    }
}