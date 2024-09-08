#include "triangle.hpp"
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

const float EPSILON = 0.0001f;

Triangle::Triangle() :
    Shape{ "Triangle" },
    v1_{ 0.0f, 0.0f, 0.0f },
    v2_{ 1.0f, 0.0f, 0.0f },
    v3_{ 0.0f, 1.0f, 0.0f }
{}

Triangle::Triangle(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3) :
    Shape{ "Triangle" },
    v1_(v1),
    v2_(v2),
    v3_(v3)
{}

Triangle::Triangle(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3, std::string const& name, std::shared_ptr<Material> const& material) :
    Shape{ name, material },
    v1_(v1),
    v2_(v2),
    v3_(v3)
{
    setMaterial(material);  // Explizite Materialzuweisung
    if (!material) {
        std::cerr << "Warnung: Kein Material für Triangle " << name << " gesetzt" << std::endl;
    }
}

Triangle::Triangle(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3, std::shared_ptr<Material> const& material) :
    Shape{ "Triangle", material },
    v1_(v1),
    v2_(v2),
    v3_(v3)
{
    setMaterial(material);  // Explizite Materialzuweisung
}

float Triangle::area() const {
    glm::vec3 edge1 = v2_ - v1_;
    glm::vec3 edge2 = v3_ - v1_;
    return 0.5f * glm::length(glm::cross(edge1, edge2));
}

float Triangle::volume() const {
    return 0.0f;
}

std::ostream& Triangle::print(std::ostream& os) const {
    Shape::print(os);
    return os << v1_.x << " " << v1_.y << " " << v1_.z << " "
        << v2_.x << " " << v2_.y << " " << v2_.z << " "
        << v3_.x << " " << v3_.y << " " << v3_.z << std::endl;
}

Hitpoint Triangle::intersectImpl(Ray const& ray) const {
    glm::vec3 edge1 = v2_ - v1_;
    glm::vec3 edge2 = v3_ - v1_;
    glm::vec3 h = glm::cross(ray.direction, edge2);
    float a = glm::dot(edge1, h);
    if (a > -EPSILON && a < EPSILON) {
        return Hitpoint(); // This ray is parallel to this triangle.
    }
    float f = 1.0f / a;
    glm::vec3 s = ray.origin - v1_;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f) {
        return Hitpoint();
    }
    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(ray.direction, q);
    if (v < 0.0f || u + v > 1.0f) {
        return Hitpoint();
    }
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * glm::dot(edge2, q);
    if (t > EPSILON) // ray intersection
    {
        Hitpoint hit;
        hit.hit = true;
        hit.distance = t;
        hit.hit_p = ray.origin + t * ray.direction;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
        if (glm::dot(normal, ray.direction) > 0) {
            normal = -normal;
        }
        hit.normal = normal;
        hit.color_obj = material_;  // Verwende das zugewiesene Material
        hit.name_obj = getName();
        return hit;
    }
    else // This means that there is a line intersection but not a ray intersection.
    {
        return Hitpoint();
    }
}