#include <vector>
#include <string>
#include "sphere.h"
#include "light.h"
#include "plane.h"

#ifndef GEOMETRY
#define GEOMETRY
class Geometry{
    public:
        std::string name;
        std::vector<Sphere> spheres;
        std::vector<Plane> planes;
        Light light;
        fmt::rgb background;

        Geometry(std::string name_, fmt::rgb background_, Light light_) : name(name_), background(background_), light(light_) {}
};
#endif