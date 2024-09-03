//#ifndef MATERIAL_HPP
//#define MATERIAL_HPP
//#include "color.hpp"
//#include <string>
//
//struct Material {
//    std::string name{ "material1" };
//    Color ka{ 0.5f, 0.5f, 0.5f };  // Ambient
//    Color kd{ 0.7f, 0.7f, 0.7f };  // Diffuse
//    Color ks{ 0.2f, 0.2f, 0.2f };  // Specular
//    float m{ 10.0f };              // Shininess
//    float refraction_index{ 1.5f }; // Refractive index
//
//    Material(
//        std::string name = "material2",
//        Color ka = Color(0.1f, 0.1f, 0.1f),
//        Color kd = Color(0.2f, 0.2f, 0.2f),
//        Color ks = Color(0.3f, 0.3f, 0.3f),
//        float m = 100.0f,
//        float refraction_index = 1.0f
//    ) : name(name), ka(ka), kd(kd), ks(ks), m(m), refraction_index(refraction_index) {}
//
//    friend std::ostream& operator<<(std::ostream& os, Material const& mat) {
//        os << mat.name << " " << mat.ka << " " << mat.kd << " " << mat.ks << " " << mat.m << " " << mat.refraction_index;
//        return os;
//    }
//};
//#endif

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

    Material(
        std::string name = "default_material",
        Color ka = Color(0.1f, 0.1f, 0.1f),
        Color kd = Color(0.7f, 0.7f, 0.7f),
        Color ks = Color(0.2f, 0.2f, 0.2f),
        float m = 10.0f,
        float refraction_index = 1.0f
    ) : name(name), ka(ka), kd(kd), ks(ks), m(m), refraction_index(refraction_index) {}

    friend std::ostream& operator<<(std::ostream& os, const Material& mat) {
        os << "Material: " << mat.name << " ka: " << mat.ka << " kd: " << mat.kd
            << " ks: " << mat.ks << " m: " << mat.m << " refraction_index: " << mat.refraction_index;
        return os;
    }

};

#endif