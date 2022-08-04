#include "hittable.hpp"
#include "vec3.hpp"
#include <iostream>
#include <common.hpp>
#include <memory>
#include <sphere.hpp>
#include <color.hpp>
#include <camera.hpp>
#include <material.hpp>
#include <aarect.hpp>
#include <box.hpp>
#include <constant_medium.hpp>
#include <bvh.hpp>

namespace raytracing {
    Color ray_color(const Ray &r, const Color &background, const Hittable &world, int depth)
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if(depth <= 0)
            return Color(0, 0, 0);

        HitRecord rec;

        // If the ray hits nothing, return the background color.
        if(!world.hit(r, 0.001, infinity, rec))
            return background;
        
        Ray scattered;
        Color attenuation;
        Color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

        if(!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return emitted;
        
        return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
    }
}

raytracing::HittableList random_scene()
{
    using namespace raytracing;
    using std::make_shared;

    HittableList world;

    auto checker =std::make_shared<CheckerTexture>(Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000,std::make_shared<Lambertian>(checker)));

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
                    sphere_material =std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material =std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material =std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 =std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 =std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 =std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));


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

raytracing::HittableList two_perlin_spheres() 
{
    using namespace raytracing;

    HittableList objects;

    auto pertext = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(Point3(0,-1000,0), 1000, std::make_shared<Lambertian>(pertext)));
    objects.add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    return objects;
}

raytracing::HittableList earth() 
{
    using namespace raytracing;

    auto earth_texture = std::make_shared<ImageTexture>("assets/earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);

    return HittableList(globe);
}

raytracing::HittableList simple_light()
{
    using namespace raytracing;

    HittableList objects;
    auto pertext = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
    objects.add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    auto difflight = std::make_shared<DiffuseLight>(Color(4, 4, 4));
    objects.add(std::make_shared<XYRect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

raytracing::HittableList cornell_box()
{
    using namespace raytracing;

    HittableList objects;

    auto red   = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Color(0.75, 0.75, 0.75));
    auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<XZRect>(213, 343, 227, 332, 554, light));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));

    std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));
    objects.add(box1);

    std::shared_ptr<Hittable> box2 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));
    objects.add(box2);

    return objects;
}

raytracing::HittableList cornell_smoke()
{
    using namespace raytracing;

    HittableList objects;

    auto red   = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(Color(0.75, 0.75, 0.75));
    auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));

    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<XZRect>(113, 443, 127, 432, 554, light));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));

    std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));
    objects.add(std::make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));

    std::shared_ptr<Hittable> box2 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));
    objects.add(std::make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

    return objects;
}

raytracing::HittableList final_scene()
{
    using namespace raytracing;

    auto boxes1 = std::make_shared<HittableList>();
    auto ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for(int i = 0; i < boxes_per_side; i++)
    {
        for(int j = 0; j < boxes_per_side; j++)
        {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1->add(std::make_shared<Box>(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
        }
    }

    HittableList objects;
    objects.add(boxes1);

    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
    objects.add(std::make_shared<XZRect>(123, 423, 147, 412, 554, light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30,0,0);
    auto moving_sphere_material =std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    objects.add(std::make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(std::make_shared<Sphere>(Point3(260, 150, 45), 50,std::make_shared<Dielectric>(1.5)));
    objects.add(std::make_shared<Sphere>(
        Point3(0, 150, 145), 50,std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary =std::make_shared<Sphere>(Point3(360,150,145), 70,std::make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary =std::make_shared<Sphere>(Point3(0, 0, 0), 5000,std::make_shared<Dielectric>(1.5));
    objects.add(std::make_shared<ConstantMedium>(boundary, .0001, Color(1,1,1)));

    auto emat =std::make_shared<Lambertian>(std::make_shared<ImageTexture>("assets/earthmap.jpg"));
    objects.add(std::make_shared<Sphere>(Point3(400,200,400), 100, emat));
    auto pertext =std::make_shared<NoiseTexture>(0.1);
    objects.add(std::make_shared<Sphere>(Point3(220,280,300), 80,std::make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white =std::make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<Sphere>(Point3::random(0,165), 10, white));
    }

    objects.add(std::make_shared<Translate>(
       std::make_shared<RotateY>(
           std::make_shared<BVHNode>(boxes2, 0.0, 1.0), 15),
            Vec3(-100,270,395)
        )
    );

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

    // Image
    auto aspect_ratio = 3.0 / 2.0;
    
    // World
    HittableList world;

    Point3 lookfrom, lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    Color background(0,0,0);
    int samples_per_pixel = 1;
    const int image_width = 200;

    switch(0) {
        case 1:
            world = random_scene();
            lookfrom = Point3(13, 2, 3);
            lookat = Point3(0, 0, 0);
            background = Color(0.70, 0.80, 1.00);
            vfov = 20.0;
            aperture = 0.1;
            break;
        
        case 2:
            world = two_spheres();
            lookfrom = Point3(13, 2, 3);
            lookat = Point3(0, 0, 0);
            background = Color(0.70, 0.80, 1.00);
            vfov = 20.0;
            break;
        
        case 3:
            world = two_perlin_spheres();
            lookfrom = Point3(13, 2, 3);
            lookat = Point3(0, 0, 0);
            background = Color(0.70, 0.80, 1.00);
            vfov = 20.0;
            break;
        
        case 4:
            world = earth();
            lookfrom = Point3(13, 2, 3);
            lookat = Point3(0, 0, 0);
            background = Color(0.70, 0.80, 1.00);
            vfov = 20.0;
            break;

        case 5:
            world = simple_light();
            samples_per_pixel = 1000;
            background = Color(0, 0, 0);
            lookfrom = Point3(26, 3, 6);
            lookat = Point3(0, 2, 0);
            vfov = 20.0;
            break;

        case 6:
            world = cornell_box();
            aspect_ratio = 1.0;
            samples_per_pixel = 200;
            background = Color(0, 0, 0);
            lookfrom = Point3(278, 278, -800);
            lookat = Point3(278, 278, 0);
            vfov = 40.0;
            break;

        case 7:
            world = cornell_smoke();
            aspect_ratio = 1.0;
            samples_per_pixel = 200;
            background = Color(0, 0, 0);
            lookfrom = Point3(278, 278, -800);
            lookat = Point3(278, 278, 0);
            vfov = 40.0;
            break;

    	default:
        case 8:
            world = final_scene();
            aspect_ratio = 1.0;
            samples_per_pixel = 1000;
            background = Color(0,0,0);
            lookfrom = Point3(478, 278, -600);
            lookat = Point3(278, 278, 0);
            vfov = 40.0;
            break;
    }

    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int max_depth = 50;

    // Camera
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

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
                pixel_color += ray_color(r, background, world, max_depth);
            }
            framebuffer[j][i] = pixel_color;
        }
    }

    write_framebuffer(std::cout, image_width, image_height, framebuffer, samples_per_pixel);

    std::cerr << "\nDone.\n";

    delete[] framebuffer;

    return 0;
}
