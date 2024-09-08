// cone.hpp
#ifndef CONE_HPP
#define CONE_HPP

#include "shape.hpp"

class Cone : public Shape {
public:
    Cone();
    Cone(std::string const& name, float radius, float height);
    Cone(std::string const& name, float radius, float height, std::shared_ptr<Material> const& material);

    float area() const override;
    float volume() const override;

protected:
    Hitpoint intersectImpl(Ray const& ray) const override;

private:
    float radius_;
    float height_;
};

#endif // CONE_HPP