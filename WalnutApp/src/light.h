#include "fmt/color.h"
#include "glm/vec3.hpp"

#ifndef LIGHT
#define LIGHT
class Light{
    public:
        glm::vec3 pos;
        fmt::rgb col;

        Light(glm::vec3 pos_, fmt::rgb col_) : pos(pos_), col(col_) {}
};
#endif