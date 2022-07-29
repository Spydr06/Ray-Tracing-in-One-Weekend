#pragma once

#include <cstdlib>
#include <cmath>
#include <limits>
#include <memory>

namespace raytracing {

    // Constants
    const double infinity = std::numeric_limits<double>::infinity();
    const double pi = 3.1415926535897932385;

    // Utility functions
    inline double degrees_to_radians(double degrees)
    {
        return degrees * pi / 180.0;
    }

    inline double random_double() 
    {
        // Returns a random real in [0,1).
        return rand() / (RAND_MAX + 1.0);
    }

    inline double random_double(double min, double max) 
    {
        // Returns a random real in [min,max).
        return min + (max - min) * random_double();
    }

    inline double clamp(double x, double min, double max)
    {
        if(x < min) 
            return min;
        if(x > max)
            return max;
        return x;
    }

} // namespace raytracing

#include "ray.hpp"
#include "vec3.hpp"