#include "hittable.hpp"
#include "vec3.hpp"
#include <iostream>
#include <common.hpp>
#include <sphere.hpp>
#include <color.hpp>
#include <camera.hpp>
#include <material.hpp>

namespace raytracing {
    Color ray_color(const Ray &r, const Hittable &world, int depth)
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if(depth <= 0)
            return Color(0, 0, 0);

        HitRecord rec;
        if(world.hit(r, 0.001, infinity, rec)) 
        {
            Ray scattered;
            Color attenuation;
            if(rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, world, depth - 1);
            
            return Color(0, 0, 0);
        }

        Vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }
}

raytracing::HittableList random_scene()
{
    using namespace raytracing;
    using std::make_shared;

    HittableList world;

    auto checker = make_shared<CheckerTexture>(Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(checker)));

    for(int a = -11; a < 11; a++)
    {
        for(int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));


    return world;
}

raytracing::HittableList two_spheres()
{
    using namespace raytracing;

    HittableList objects;
    auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    objects.add(std::make_shared<Sphere>(Point3(0,-10, 0), 10, std::make_shared<Lambertian>(checker)));
    objects.add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

    return objects;
}

template<size_t SZ>
void write_framebuffer(std::ostream &out, int image_width, int image_height, raytracing::Color framebuffer[][SZ], int samples_per_pixel)
{
    for(int j = image_height - 1; j >= 0; --j)
    {
        for(int i = 0; i < image_width; ++i)
        {
            write_color(std::cout, framebuffer[j][i], samples_per_pixel);
        }
    }
    out << std::flush;
}


int main(int argc, char* argv[]) 
{
    using namespace raytracing;
    using std::make_shared;

    // Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 300;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    /*HittableList world;

    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<Dielectric>(1.5);
    auto material_right  = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));
    */
    
    // World
    HittableList world;

    Point3 lookfrom, lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;

    switch(0) {
        case 1:
            world = random_scene();
            lookfrom = Point3(13, 2, 3);
            lookat = Point3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        
        default:
        case 2:
            world = two_spheres();
            lookfrom = Point3(13, 2, 3);
            lookat = Point3(0, 0, 0);
            vfov = 20.0;
            break;
    }

    // Camera
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 0.0);

    // Render

    auto framebuffer = new Color[image_height][image_width];

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < image_width; ++i) 
        {
            Color pixel_color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; ++s) 
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            framebuffer[j][i] = pixel_color;
        }
    }

    write_framebuffer(std::cout, image_width, image_height, framebuffer, samples_per_pixel);

    std::cerr << "\nDone.\n";

    delete[] framebuffer;

    return 0;
}