#ifndef SHAPE_HPP
#define SHAPE_HPP
#include <string>
#include <ostream>
#include <glm/vec3.hpp>
#include "Hitpoint.hpp"
#include "Ray.hpp"
#include "Material.hpp"

class Shape {
public:
	std::string name_;
	std::shared_ptr<Material> color_;

	virtual float area()const = 0;
	virtual float volume()const = 0;
	
	Shape();
	Shape(std::string name, std::shared_ptr<Material> color);
	Shape(std::string name);
	virtual ~Shape();

	virtual std::ostream& print(std::ostream& os) const;
	virtual Hitpoint intersect(Ray const& ray) = 0;
};
std::ostream& operator<<(std::ostream& os, Shape const& s);
#endif