#pragma once

#include "common.hpp"
#include "vec3.hpp"

namespace raytracing {

class Perlin {
    public:
        Perlin();
        ~Perlin();
        
        double noise(const Point3 &p) const;
        double turb(const Point3 &p, int depth = 7) const;
    
    private:
        static int* perlin_generate_perm();
        static void permute(int* p, int n);
        static double trilinear_interp(double c[2][2][2], double u, double v, double w);
        static double perlin_interp(Vec3 c[2][2][2], double u, double v, double w);

    private:
        static const int point_count = 256;
        Vec3* ranvec;
        int* perm_x;
        int* perm_y;
        int* perm_z;
};

} // namespace raytracing