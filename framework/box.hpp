// box.hpp
#ifndef BOX_HPP
#define BOX_HPP
#include "shape.hpp"
#include <glm/vec3.hpp>

class Box : public Shape {
private:
    /*glm::vec3 min_{ 0.0f, 0.0f, 0.0f };
    glm::vec3 max_{ 100.0f, 100.0f, 100.0f };*/
public:
    Box();
    Box(glm::vec3 min, glm::vec3 max);
    Box(glm::vec3 min, glm::vec3 max, std::string const& name, std::shared_ptr<Material> const& color);
    Box(glm::vec3 min, glm::vec3 max, std::shared_ptr<Material> const& material);
    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const override;

    glm::vec3 min_{ 0.0f, 0.0f, 0.0f };
    glm::vec3 max_{ 100.0f, 100.0f, 100.0f };
protected:
    Hitpoint intersectImpl(Ray const& ray) const override;
};
#endif