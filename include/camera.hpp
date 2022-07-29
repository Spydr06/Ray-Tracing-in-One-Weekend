#pragma once

#include "common.hpp"
#include "vec3.hpp"
#include "ray.hpp"

namespace raytracing {

class Camera {
    public:
        Camera(
            Point3 lookfrom, 
            Point3 lookat, 
            Vec3 vup, 
            double vfov /*vertical Field-Of-View*/, 
            double aspect_ratio,
            double aperture,
            double focus_dist
        )
        {
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta / 2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

            lens_radius = aperture / 2;
        }

        Ray get_ray(double s, double t) const
        {
            Vec3 rd = lens_radius * random_in_unit_disk();
            Vec3 offset = u * rd.x() + v * rd.y();

            return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
        }
    
    private:
        Point3 origin, lower_left_corner;
        Vec3 horizontal, vertical, u, v, w;
        double lens_radius;
};

} // namespace raytracing