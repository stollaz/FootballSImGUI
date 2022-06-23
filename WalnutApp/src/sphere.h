#include "fmt/color.h"
#include "glm/vec3.hpp"
#include <functional>

#ifndef SPHERE
#define SPHERE
class Sphere{
    public:
        float r;
        glm::vec3 centre;
        fmt::rgb col;

        // I want some way to define a function to say how the colour should be returned, but the function can be defined per-sphere upon instantiation
        // e.g. getColour(x,y,z) { return fmt::rgb(255*(normal.x+1)/2,255*(normal.y+1)/2,255*(normal.z+1)/2); }
        // Try this? https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp/

        Sphere(float r_, glm::vec3 centre_, fmt::rgb col_) : r(r_), centre(centre_), col(col_) {}
};
#endif