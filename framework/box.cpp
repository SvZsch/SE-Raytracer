#include "box.hpp"
#include "cmath"
#include "ostream"
#include "Hitpoint.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <algorithm>

const float EPSILON = 0.0001f;

Box::Box() :
    Shape{ "Quader" },
    min_{ 0.0f, 0.0f , 0.0f },
    max_{ 100.0f, 100.0f, 100.0f }
{}

Box::Box(glm::vec3 min, glm::vec3 max) :
    Shape{ "Quader" },
    min_(min),
    max_(max)
{}

Box::Box(glm::vec3 min, glm::vec3 max, std::string const& name, std::shared_ptr<Material> const& material) :
    Shape{ name, material },
    min_(min),
    max_(max)
{
    if (!material) {
        //std::cerr << "Warnung: Kein Material für Box " << name << " gesetzt" << std::endl;
    }
}

Box::Box(glm::vec3 min, glm::vec3 max, std::shared_ptr<Material> const& material) :
    Shape{ "Quader", material },
    min_(min),
    max_(max)
{}

float Box::area() const {
    float a = abs(max_.x - min_.x);
    float b = abs(max_.y - min_.y);
    float c = abs(max_.z - min_.z);
    return 2 * a * b + 2 * a * c + 2 * b * c;
}

float Box::volume() const {
    return abs(max_.x - min_.x) * abs(max_.y - min_.y) * abs(max_.z - min_.z);
}

std::ostream& Box::print(std::ostream& os) const {
    Shape::print(os);
    return os << min_.x << " " << min_.y << " " << min_.z << " " << max_.x << " " << max_.y << " " << max_.z << std::endl;
}

// box.cpp
Hitpoint Box::intersectImpl(Ray const& ray) const {
    glm::vec3 t_min = (min_ - ray.origin) / ray.direction;
    glm::vec3 t_max = (max_ - ray.origin) / ray.direction;
    glm::vec3 t1 = glm::min(t_min, t_max);
    glm::vec3 t2 = glm::max(t_min, t_max);
    float t_near = glm::max(glm::max(t1.x, t1.y), t1.z);
    float t_far = glm::min(glm::min(t2.x, t2.y), t2.z);

    //std::cout << "t_near: " << t_near << ", t_far: " << t_far << std::endl;

    Hitpoint hit;
    if (t_far > t_near && t_far > 0) {
        hit.hit = true;
        hit.distance = (t_near > 0) ? t_near : t_far;
        hit.hit_p = ray.origin + hit.distance * ray.direction;

        // Berechnung der Normalen
        if (std::abs(hit.hit_p.x - min_.x) < EPSILON) hit.normal = glm::vec3(-1, 0, 0);
        else if (std::abs(hit.hit_p.x - max_.x) < EPSILON) hit.normal = glm::vec3(1, 0, 0);
        else if (std::abs(hit.hit_p.y - min_.y) < EPSILON) hit.normal = glm::vec3(0, -1, 0);
        else if (std::abs(hit.hit_p.y - max_.y) < EPSILON) hit.normal = glm::vec3(0, 1, 0);
        else if (std::abs(hit.hit_p.z - min_.z) < EPSILON) hit.normal = glm::vec3(0, 0, -1);
        else hit.normal = glm::vec3(0, 0, 1);

        hit.color_obj = material_;
        hit.name_obj = getName();

        //std::cout << "Hit found at distance: " << hit.distance << std::endl;
        //std::cout << "Hit point: (" << hit.hit_p.x << ", " << hit.hit_p.y << ", " << hit.hit_p.z << ")" << std::endl;
        //std::cout << "Normal: (" << hit.normal.x << ", " << hit.normal.y << ", " << hit.normal.z << ")" << std::endl;
    }
    else {
        hit.hit = false;
        //std::cout << "No hit found" << std::endl;
    }

    return hit;
}