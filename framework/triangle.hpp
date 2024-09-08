#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"
#include <glm/vec3.hpp>

class Triangle : public Shape {
public:
    Triangle();
    Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
    Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
        const std::string& name, const std::shared_ptr<Material>& material);
    Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
        const std::shared_ptr<Material>& material);

    //// Kopier-Konstruktor
    //Triangle(const Triangle& other);

    //// Zuweisungsoperator
    //Triangle& operator=(const Triangle& other);

    //// Verschiebekonstruktor
    //Triangle(Triangle&& other) noexcept;

    //// Verschiebezuweisungsoperator
    //Triangle& operator=(Triangle&& other) noexcept;

    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const override;

protected:
    Hitpoint intersectImpl(Ray const& ray) const override;

private:
    glm::vec3 v1_{ 0.0f, 0.0f, 0.0f };
    glm::vec3 v2_{ 1.0f, 0.0f, 0.0f };
    glm::vec3 v3_{ 0.0f, 1.0f, 0.0f };
};

#endif