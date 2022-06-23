#include "fmt/color.h"
#include <string>

#ifndef PLANE
#define PLANE
class Plane{
    public:
        std::string id;
        glm::vec3 normal;
        int d;
        fmt::rgb col;

        Plane(std::string id_, glm::vec3 normal_, int d_, fmt::rgb col_) : id(id_), normal(normal_), d(d_), col(col_) {}
};
#endif