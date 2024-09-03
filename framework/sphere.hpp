#ifndef SPHERE_HPP
#define SPHERE_HPP
#include "shape.hpp"
#include <glm/vec3.hpp>
#include "Hitpoint.hpp"
#include "Ray.hpp"
#include <Hitpoint.hpp>

class Sphere : public Shape {
private:
	float rad{ 10.0f };
	glm::vec3 point{ 100.0f, 100.0f, 100.0f };
public:
	Sphere();
	~Sphere();
	Sphere(float r, glm::vec3 p);
	Sphere(float r, glm::vec3 p, std::string name, std::shared_ptr<Material> color);
	float Shape::area()const override;
	float Shape::volume()const override;
	std::ostream& print(std::ostream& os)const;
	Hitpoint intersect(Ray const& ray) override;
};
#endif