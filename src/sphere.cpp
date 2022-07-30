#include <cmath>
#include <sphere.hpp>

namespace raytracing {
    bool Sphere::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
    {
        Vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if(discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if(root < t_min || root > t_max)
        {
            root = (-half_b + sqrtd) / a;
            if(root < t_min || root > t_max)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat_ptr = mat_ptr;

        return true;
    }

    bool Sphere::bounding_box(double time0, double time1, AABB &output_box) const
    {
        output_box = AABB(
            center - Vec3(radius, radius, radius),
            center + Vec3(radius, radius, radius)
        );
        return true;
    }

    void Sphere::get_sphere_uv(const Point3 &p, double &u, double &v) 
    {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }

    Point3 MovingSphere::center(double time) const 
    {
        return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
    }

    bool MovingSphere::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
    {
        Vec3 oc = r.origin() - center(r.time());
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if(discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if(root < t_min || root > t_max)
        {
            root = (-half_b + sqrtd) / a;
            if(root < t_min || root > t_max)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center(r.time())) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;

        return true;
    }

    bool MovingSphere::bounding_box(double _time0, double _time1, AABB &output_box) const
    {
        AABB box0(
            center(_time0) - Vec3(radius, radius, radius),
            center(_time0) + Vec3(radius, radius, radius));
        AABB box1(
            center(_time1) - Vec3(radius, radius, radius),
            center(_time1) + Vec3(radius, radius, radius));
        output_box = surrounding_box(box0, box1);
        return true;
    }
}