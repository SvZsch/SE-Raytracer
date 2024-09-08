// cone.cpp
#include "cone.hpp"
#include "Hitpoint.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <algorithm>

const double M_PI = 3.14159265358979323846;


Cone::Cone() : Shape(), radius_(1.0f), height_(1.0f) {
    //std::cout << "Cone default constructor called" << std::endl;
}

Cone::Cone(std::string const& name, float radius, float height)
    : Shape(name), radius_(radius), height_(height) {
    //std::cout << "Cone constructor called: " << name << std::endl;
}

Cone::Cone(std::string const& name, float radius, float height, std::shared_ptr<Material> const& material)
    : Shape(name, material), radius_(radius), height_(height) {
    //std::cout << "Cone constructor with material called: " << name << std::endl;
    setMaterial(material);  // Explizit das Material setzen
    if (material) {
        //std::cout << "Material set: " << material->name << std::endl;
    }
    else {
        //std::cout << "Warning: Null material provided for Cone " << name << std::endl;
    }
}

float Cone::area() const {
    return M_PI * radius_ * (radius_ + std::sqrt(height_ * height_ + radius_ * radius_));
}

float Cone::volume() const {
    return (1.0f / 3.0f) * M_PI * radius_ * radius_ * height_;
}

// In cone.cpp
Hitpoint Cone::intersectImpl(Ray const& ray) const {
    Hitpoint result;

    // Transformiere den Strahl in das lokale Koordinatensystem des Kegels
    glm::vec3 co = ray.origin - glm::vec3(0, 0, 0);  // Kegel-Ursprung ist jetzt an (0,0,0)
    glm::vec3 dir = glm::normalize(ray.direction);

    float k = radius_ / height_;
    float k2 = k * k;

    float a = dir.x * dir.x + dir.z * dir.z - k2 * dir.y * dir.y;
    float b = 2 * (co.x * dir.x + co.z * dir.z - k2 * co.y * dir.y + k2 * height_ * dir.y);
    float c = co.x * co.x + co.z * co.z - k2 * (co.y * co.y - 2 * height_ * co.y + height_ * height_);

    float discriminant = b * b - 4 * a * c;

    if (std::abs(a) < 1e-6 || discriminant < 0) {
        return result;  // Kein Schnittpunkt
    }

    float sqrt_discriminant = std::sqrt(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2 * a);
    float t2 = (-b + sqrt_discriminant) / (2 * a);

    // Sortiere t1 und t2, sodass t1 <= t2
    if (t1 > t2) std::swap(t1, t2);

    // Prüfe auch den Schnitt mit der Grundfläche
    float t_base = -co.y / dir.y;

    float t = std::numeric_limits<float>::max();
    glm::vec3 normal;
    bool hit_base = false;

    // Prüfe Mantelfläche
    auto check_mantle = [&](float t_check) {
        if (t_check > 0) {
            glm::vec3 p = co + t_check * dir;
            if (p.y >= 0 && p.y <= height_) {
                t = t_check;
                return true;
            }
        }
        return false;
        };

    // Prüfe zuerst t1, dann t2
    if (check_mantle(t1)) {
        // Erster Schnittpunkt ist gültig
    }
    else if (check_mantle(t2)) {
        // Zweiter Schnittpunkt ist gültig
    }
    else if (t_base > 0 && t_base < t) {
        // Prüfe Grundfläche
        glm::vec3 p = co + t_base * dir;
        if (p.x * p.x + p.z * p.z <= radius_ * radius_) {
            t = t_base;
            hit_base = true;
        }
    }

    if (t < std::numeric_limits<float>::max()) {
        result.hit = true;
        result.distance = t;
        result.hit_p = ray.origin + t * dir;

        if (hit_base) {
            normal = glm::vec3(0, -1, 0);
        }
        else {
            // Berechne die Normale für die Mantelfläche
            float y = result.hit_p.y;
            float r = std::sqrt(result.hit_p.x * result.hit_p.x + result.hit_p.z * result.hit_p.z);
            normal = glm::normalize(glm::vec3(result.hit_p.x, r * k, result.hit_p.z));

            // Behandlung der Kegelspitze
            if (y >= height_ - 1e-4f) {
                normal = glm::vec3(0, 1, 0);
            }
        }

        result.normal = normal;
        result.color_obj = material_;
        result.name_obj = getName();
    }

    return result;
}
