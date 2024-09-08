// cylinder.cpp
#include "cylinder.hpp"
#include <cmath>
#include <algorithm>

const double M_PI = 3.14159265358979323846;

Cylinder::Cylinder() : Shape(), radius_(1.0f), height_(1.0f) {}

Cylinder::Cylinder(std::string const& name, float radius, float height)
    : Shape(name), radius_(radius), height_(height) {}

Cylinder::Cylinder(std::string const& name, float radius, float height, std::shared_ptr<Material> const& material)
    : Shape(name, material), radius_(radius), height_(height) {
    setMaterial(material);
}

float Cylinder::area() const {
    return 2 * M_PI * radius_ * height_ + 2 * M_PI * radius_ * radius_;
}

float Cylinder::volume() const {
    return M_PI * radius_ * radius_ * height_;
}

Hitpoint Cylinder::intersectImpl(Ray const& ray) const {
    Hitpoint result;

    // Seitenberechnung des Zylinders
    float a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
    float b = 2 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z);
    float c = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - radius_ * radius_;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return result;
    }

    float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
    float t2 = (-b + std::sqrt(discriminant)) / (2 * a);

    // Deckflächenberechnung
    float t_bottom = (0 - ray.origin.y) / ray.direction.y;
    float t_top = (height_ - ray.origin.y) / ray.direction.y;

    // Finde den nächsten gültigen Schnittpunkt
    float t = std::numeric_limits<float>::max();
    glm::vec3 normal;

    // Prüfe Seitenfläche
    if (t1 > 0 && t1 < t) {
        glm::vec3 p = ray.origin + t1 * ray.direction;
        if (p.y >= 0 && p.y <= height_) {
            t = t1;
            normal = glm::normalize(glm::vec3(p.x, 0, p.z));
        }
    }
    if (t2 > 0 && t2 < t) {
        glm::vec3 p = ray.origin + t2 * ray.direction;
        if (p.y >= 0 && p.y <= height_) {
            t = t2;
            normal = glm::normalize(glm::vec3(p.x, 0, p.z));
        }
    }

    // Prüfe untere Deckfläche
    if (t_bottom > 0 && t_bottom < t) {
        glm::vec3 p = ray.origin + t_bottom * ray.direction;
        if (p.x * p.x + p.z * p.z <= radius_ * radius_) {
            t = t_bottom;
            normal = glm::vec3(0, -1, 0);
        }
    }

    // Prüfe obere Deckfläche
    if (t_top > 0 && t_top < t) {
        glm::vec3 p = ray.origin + t_top * ray.direction;
        if (p.x * p.x + p.z * p.z <= radius_ * radius_) {
            t = t_top;
            normal = glm::vec3(0, 1, 0);
        }
    }

    if (t < std::numeric_limits<float>::max()) {
        result.hit = true;
        result.distance = t;
        result.hit_p = ray.origin + t * ray.direction;
        result.normal = normal;
        result.color_obj = material_;
        result.name_obj = getName();
    }

    return result;
}