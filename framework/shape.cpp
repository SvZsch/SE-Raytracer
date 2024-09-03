#include "shape.hpp"
#include <iostream>


Shape::Shape(): name_{ "Körper" }, color_() {}

Shape::Shape(std::string name, std::shared_ptr<Material> color):
	name_(name),
	color_(color) 
{
	std::cout << "Shape ctor" << std::endl;
}

Shape::Shape(std::string name) :
	name_(name),
	color_{}
{
	std::cout << "Shape ctor" << std::endl;
}

Shape::~Shape()
{
	std::cout << "Shape dtor" << std::endl;
}

std::ostream& Shape::print(std::ostream& os)const {
	return os << name_ << " " << " " << color_ << " "  << std::endl;
}

std::ostream& operator<<(std::ostream & os, Shape const& s)
{
	return s.print(os);
}
