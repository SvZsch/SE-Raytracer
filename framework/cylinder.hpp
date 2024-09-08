// cylinder.hpp
#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "shape.hpp"

class Cylinder : public Shape {
public:
    Cylinder();
    Cylinder(std::string const& name, float radius, float height);
    Cylinder(std::string const& name, float radius, float height, std::shared_ptr<Material> const& material);

    float area() const override;
    float volume() const override;

protected:
    Hitpoint intersectImpl(Ray const& ray) const override;

private:
    float radius_;
    float height_;
};

#endif // CYLINDER_HPP