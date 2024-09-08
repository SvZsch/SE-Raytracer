// composite.cpp
#include "composite.hpp"
#include <algorithm>

Composite::Composite(const std::string& name) : Shape(name) {}

void Composite::add(std::shared_ptr<Shape> shape) {
    //std::cout << "Adding shape " << shape->getName() << " to composite " << getName() << std::endl;
    children_.push_back(shape);
    //std::cout << "Composite " << getName() << " now has " << children_.size() << " children" << std::endl;
}

void Composite::remove(std::shared_ptr<Shape> shape) {
    children_.erase(std::remove(children_.begin(), children_.end(), shape), children_.end());
}

float Composite::area() const {
    float total_area = 0.0f;
    for (const auto& child : children_) {
        total_area += child->area();
    }
    return total_area;
}

float Composite::volume() const {
    float total_volume = 0.0f;
    for (const auto& child : children_) {
        total_volume += child->volume();
    }
    return total_volume;
}

std::ostream& Composite::print(std::ostream& os) const {
    Shape::print(os);
    //os << "Composite with " << children_.size() << " children:" << std::endl;
    for (const auto& child : children_) {
        child->print(os);
    }
    return os;
}

Hitpoint Composite::intersectImpl(Ray const& ray) const {
    //std::cout << "Intersecting Composite " << getName() << std::endl;
    Hitpoint closest_hit;
    closest_hit.distance = std::numeric_limits<float>::max();
    for (const auto& child : children_) {
        Hitpoint hit = child->intersect(ray);
        if (hit.hit && hit.distance < closest_hit.distance) {
            closest_hit = hit;
        }
    }
    if (closest_hit.hit) {
        //std::cout << "Composite " << getName() << " hit detected at distance " << closest_hit.distance << std::endl;
    }
    else {
        //std::cout << "No hit for Composite " << getName() << std::endl;
    }
    return closest_hit;
}