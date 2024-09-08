// sphere.hpp
#ifndef SPHERE_HPP
#define SPHERE_HPP
#include "shape.hpp"
#include <glm/vec3.hpp>
#include "Hitpoint.hpp"
#include "Ray.hpp"

class Sphere : public Shape {
private:
    /*float rad{ 10.0f };
    glm::vec3 point{ 100.0f, 100.0f, 100.0f };*/
public:
    Sphere();
    ~Sphere();
    Sphere(float r, glm::vec3 p);
    Sphere(float r, glm::vec3 p, std::string const& name, std::shared_ptr<Material> const& color);
    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const override;
    float rad{ 10.0f };
    glm::vec3 point{ 100.0f, 100.0f, 100.0f };
protected:
    Hitpoint intersectImpl(Ray const& ray) const override;
};
#endif