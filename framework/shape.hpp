// shape.hpp
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Ray.hpp"
#include "Hitpoint.hpp"
#include "Material.hpp"

//const float EPSILON = 0.0001f;

class Shape {
public:
    Shape();
    Shape(std::string const& name);
    Shape(std::string const& name, std::shared_ptr<Material> const& color);
    virtual ~Shape() = default;

    virtual float area() const = 0;
    virtual float volume() const = 0;
    virtual std::ostream& print(std::ostream& os) const;

    Hitpoint intersect(Ray const& ray) const;
    void setTransformation(glm::mat4 const& transformation);
    const std::string& getName() const;
    const glm::mat4& getWorldTransformation() const;

    void setMaterial(std::shared_ptr<Material> const& material) {
        material_ = material;
    }

    // Neue Methoden für Composite-Funktionalität
    virtual void add(std::shared_ptr<Shape> shape) {}
    virtual void remove(std::shared_ptr<Shape> shape) {}

protected:
    virtual Hitpoint intersectImpl(Ray const& ray) const = 0;
    Ray transformRay(Ray const& ray) const;
    glm::vec3 transformPoint(glm::vec3 const& point) const;
    glm::vec3 transformNormal(glm::vec3 const& normal, glm::vec3 const& point) const;

    std::string name_;
    std::shared_ptr<Material> color_;
    std::shared_ptr<Material> material_;
    glm::mat4 world_transformation_;
    glm::mat4 world_transformation_inv_;
};

std::ostream& operator<<(std::ostream& os, Shape const& s);

#endif // SHAPE_HPP