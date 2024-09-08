#include "sphere.hpp"
#include "Hitpoint.hpp"
#include <numbers>
#include <cmath>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <glm/gtx/intersect.hpp>
#include <iostream>

Sphere::Sphere() :
    Shape{ "Kugel" },
    rad{ 10.0f },
    point{ 100.0f, 100.0f, 100.0f }
{
    std::cout << "Sphere ctor" << std::endl;
}

Sphere::~Sphere()
{
    std::cout << "Sphere dtor" << std::endl;
}

Sphere::Sphere(float r, glm::vec3 p) :
    Shape{ "Kugel" },
    rad(r),
    point(p)
{
    std::cout << "Sphere ctor" << std::endl;
}

Sphere::Sphere(float r, glm::vec3 p, std::string const& name, std::shared_ptr<Material> const& color) :
    Shape{ name, color },
    rad(r),
    point(p)
{
    std::cout << "Sphere ctor" << std::endl;
}

float Sphere::volume() const {
    return abs((4.0f / 3.0f) * std::numbers::pi * rad * rad * rad);
}

float Sphere::area() const {
    return abs(4.0f * std::numbers::pi * rad * rad);
}

std::ostream& Sphere::print(std::ostream& os) const {
    Shape::print(os);
    return os << rad << " " << point.x << " " << point.y << " " << point.z << std::endl;
}

// Hilfsfunktion zum Konvertieren von glm::vec3 in einen String
std::string vec3_to_string(const glm::vec3& v) {
    std::stringstream ss;
    ss << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return ss.str();
}

//Hitpoint Sphere::intersectImpl(Ray const& ray) const {
//    std::cout << "Sphere::intersectImpl called for " << getName() << std::endl;
//    std::cout << "Sphere center: (" << point.x << ", " << point.y << ", " << point.z << "), radius: " << rad << std::endl;
//    std::cout << "Ray origin: (" << ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z
//        << "), direction: (" << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << ")" << std::endl;
//    
//    auto v = glm::normalize(ray.direction);
//    float dist = 0.0f;
//    if (glm::intersectRaySphere(ray.origin, v, point, rad * rad, dist)) {
//        Hitpoint hitpoint;
//        hitpoint.hit = true;
//        hitpoint.distance = dist;
//        hitpoint.name_obj = name_;
//        hitpoint.color_obj = color_;
//
//        const float EPSILON = 0.0001f;
//        glm::vec3 intersection_point = ray.origin + (dist - EPSILON) * ray.direction;
//        hitpoint.hit_p = intersection_point;
//
//        float n1 = 1.0f; // Brechungsindex von Luft
//        float n2 = color_->refraction_index;
//        float eta = n1 / n2;
//        glm::vec3 normal = glm::normalize(hitpoint.hit_p - point);
//
//        if (glm::dot(ray.direction, normal) > 0) {
//            // Strahl tritt aus der Kugel aus
//            normal = -normal;
//            std::swap(n1, n2);
//            eta = n1 / n2;
//        }
//
//        hitpoint.normal = normal;
//        hitpoint.reflection_dir = glm::reflect(ray.direction, normal);
//        hitpoint.refraction_dir = glm::refract(ray.direction, normal, eta);
//
//        if (hit) {
//            std::cout << "Hit found at distance: " << dist << std::endl;
//        }
//        else {
//            std::cout << "No hit found" << std::endl;
//        }
//
//        return hitpoint;
//    }
//    else {
//        return Hitpoint(); // Keine Kollision
//    }
//}

Hitpoint Sphere::intersectImpl(Ray const& ray) const {
    glm::vec3 oc = ray.origin - point;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(oc, ray.direction);
    float c = glm::dot(oc, oc) - rad * rad;
    float discriminant = b * b - 4 * a * c;

    //std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;
    //std::cout << "Discriminant: " << discriminant << std::endl;

    Hitpoint hitpoint;
    if (discriminant >= 0) {
        float dist = (-b - std::sqrt(discriminant)) / (2.0f * a);
        if (dist > 0) {
            hitpoint.hit = true;
            hitpoint.distance = dist;
            hitpoint.hit_p = ray.origin + dist * ray.direction;
            hitpoint.normal = glm::normalize(hitpoint.hit_p - point);
            hitpoint.color_obj = color_;
            hitpoint.name_obj = getName();
            //std::cout << "Hit found at distance: " << dist << std::endl;
        }
        else {
            //std::cout << "Hit behind ray origin" << std::endl;
        }
    }
    else {
        //std::cout << "No hit found" << std::endl;
    }

    return hitpoint;
}