// shape.cpp
#include "shape.hpp"
#include "Hitpoint.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Shape::Shape() : name_("Unnamed"), color_(nullptr), world_transformation_(1.0f), world_transformation_inv_(1.0f) {}

Shape::Shape(std::string const& name) : name_(name), color_(nullptr), world_transformation_(1.0f), world_transformation_inv_(1.0f) {}

Shape::Shape(std::string const& name, std::shared_ptr<Material> const& color) : name_(name), color_(color), world_transformation_(1.0f), world_transformation_inv_(1.0f) {}

std::ostream& Shape::print(std::ostream& os) const {
    return os << name_ << " " << " " << color_ << " " << std::endl;
}

void Shape::setTransformation(glm::mat4 const& transformation) {
    world_transformation_ = transformation * world_transformation_;
    world_transformation_inv_ = glm::inverse(world_transformation_);
}

Ray Shape::transformRay(Ray const& ray) const {
    Ray transformedRay;
    transformedRay.origin = glm::vec3(world_transformation_inv_ * glm::vec4(ray.origin, 1.0f));
    transformedRay.direction = glm::normalize(glm::vec3(world_transformation_inv_ * glm::vec4(ray.direction, 0.0f)));
    return transformedRay;
}

glm::vec3 Shape::transformPoint(glm::vec3 const& point) const {
    return glm::vec3(world_transformation_ * glm::vec4(point, 1.0f));
}

glm::vec3 Shape::transformNormal(glm::vec3 const& normal, glm::vec3 const& point) const {
    // Berechnen Sie die inverse Transponierte der Jacobi-Matrix an diesem Punkt
    glm::mat3 invTranspose = glm::inverseTranspose(glm::mat3(world_transformation_));
    return glm::normalize(invTranspose * normal);
}

Hitpoint Shape::intersect(Ray const& ray) const {
    Ray transformedRay = transformRay(ray);
    transformedRay.direction = glm::normalize(transformedRay.direction);

    Hitpoint hit = intersectImpl(transformedRay);

    if (hit.hit) {
        hit.hit_p = transformPoint(hit.hit_p);
        hit.normal = glm::normalize(transformNormal(hit.normal, hit.hit_p));
        hit.distance = glm::length(hit.hit_p - ray.origin);
    }
    return hit;
}

const std::string& Shape::getName() const {
    return name_;
}

const glm::mat4& Shape::getWorldTransformation() const {
    return world_transformation_;
}

std::ostream& operator<<(std::ostream& os, Shape const& s) {
    return s.print(os);
}