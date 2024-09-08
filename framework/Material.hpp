// Material.hpp
#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "color.hpp"
#include <string>

struct Material {
    std::string name;
    Color ka;  // Ambient
    Color kd;  // Diffuse
    Color ks;  // Specular
    float m;   // Shininess
    float refraction_index; // Refractive index
    float transparency;     // Transparency (0.0 = opaque, 1.0 = fully transparent)

    Color emission;

    Material(
        std::string name = "default_material",
        Color ka = Color(0.1f, 0.1f, 0.1f),
        Color kd = Color(0.7f, 0.7f, 0.7f),
        Color ks = Color(0.2f, 0.2f, 0.2f),
        float m = 10.0f,
        float refraction_index = 1.0f,
        float transparency = 0.0f
    ) : name(name), ka(ka), kd(kd), ks(ks), m(m),
        refraction_index(refraction_index), transparency(transparency) {}

    friend std::ostream& operator<<(std::ostream& os, const Material& mat) {
        os << "Material: " << mat.name << " ka: " << mat.ka << " kd: " << mat.kd
            << " ks: " << mat.ks << " m: " << mat.m << " refraction_index: " << mat.refraction_index
            << " transparency: " << mat.transparency;
        return os;
    }
};
#endif