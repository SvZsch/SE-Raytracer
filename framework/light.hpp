////Aufgabe 7 hinzugefügt
//
//#ifndef LIGHT_HPP
//#define LIGHT_HPP
//
//#include <glm/vec3.hpp>
//#include "color.hpp"
//
//class Light {
//public:
//    Light(glm::vec3 const& pos, Color const& col)
//        : position(pos), color(col) {}
//
//    glm::vec3 position;
//    Color color;
//};
//
//#endif // LIGHT_HPP

// Light.hpp
#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/vec3.hpp>
#include "color.hpp"
#include <string>

struct Light
{
    glm::vec3 position;
    Color color;
    std::string name;

    Light(glm::vec3 const& pos, Color const& col, std::string const& n = "default_light")
        : position(pos), color(col), name(n) {}
};

#endif