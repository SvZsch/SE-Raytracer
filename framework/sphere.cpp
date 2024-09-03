#include "sphere.hpp"
#include "Hitpoint.hpp"
#include <numbers>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>

Sphere::Sphere():
	Shape::Shape{"Kugel"},
    rad{ 10.0f },
	point{ 100.0f, 100.0f, 100.0f }
{std::cout << "Sphere ctor" << std::endl; }

Sphere::~Sphere()
{
	std::cout << "Sphere dtor" << std::endl;
}

Sphere::Sphere(float r, glm::vec3 p) :
	Shape::Shape{"Kugel"},
	rad(r),
	point(p)
{
	std::cout << "Sphere ctor" << std::endl;
}

Sphere::Sphere(float r, glm::vec3 p, std::string name, std::shared_ptr<Material> color):
	Shape::Shape{name, color},
	rad(r),
	point(p)
{
	std::cout << "Sphere ctor" << std::endl;
}


float Sphere::volume()const {
	return abs((4.0f / 3.0f) * std::numbers::pi * rad * rad * rad);
}

float Sphere::area()const {
	return abs(4.0f * std::numbers::pi * rad * rad);
}


std::ostream& Sphere::print(std::ostream& os)const {
	Shape::print(os);
	return os << rad << " " << point.x << " " << point.y << " " << point.z << std::endl;
}

Hitpoint Sphere::intersect(Ray const& ray) {
	auto v = glm::normalize(ray.direction);
	float dist = 0.0f;

	/*if (glm::intersectRaySphere(ray.origin, v, point, rad * rad, dist)) {
		Hitpoint hitpoint;
		hitpoint.hit = true;
		hitpoint.distance = dist;
		hitpoint.name_obj = name_;
		hitpoint.color_obj = color_;

		float a = (ray.direction.x) * (ray.direction.x) +
			(ray.direction.y) * (ray.direction.y) + (ray.direction.z) * (ray.direction.z);
		float b = 2 * ((ray.origin.x - point.x) * ray.direction.x +
			+ (ray.origin.y - point.y) * ray.direction.y + (ray.origin.z - point.z) * ray.direction.z);
		float c = ((ray.origin.x - point.x) * (ray.origin.x - point.x) +
			(ray.origin.y - point.y) * (ray.origin.y - point.y) +
			(ray.origin.z - point.z) * (ray.origin.z - point.z)) - (rad * rad);
		std::cout << "hier " << a << " " << b << " " << c << std::endl;

	

		if ((b * b - 4 * a * c) == 0) {
			float t = (-b) / (2 * a);
			glm::vec3 treffer = { ray.origin.x + (t * ray.direction.x), ray.origin.y + (t * ray.direction.y),
			ray.origin.z + (t * ray.direction.z) };
			hitpoint.hit_p = treffer;
		}
		else {
			float t1 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
			float t2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
			glm::vec3 treffer = { ray.origin.x + (t1 * ray.direction.x), ray.origin.y + (t1 * ray.direction.y),
			ray.origin.z + (t1 * ray.direction.z) };
			hitpoint.hit_p = treffer;
			

		}
		hitpoint.ray_dir = ray.direction;
		return hitpoint;
	}
	else {
		Hitpoint hitpoint;
		hitpoint.hit = false;
		return hitpoint;
	}*/

	//hinzugefügt
	if (glm::intersectRaySphere(ray.origin, v, point, rad * rad, dist)) {
		Hitpoint hitpoint;
		hitpoint.hit = true;
		hitpoint.distance = dist;
		hitpoint.name_obj = name_;
		hitpoint.color_obj = color_;

		// Berechne den Schnittpunkt und die Normale
		glm::vec3 intersection_point = ray.origin + dist * ray.direction;
		hitpoint.hit_p = intersection_point;
		hitpoint.normal = glm::normalize(hitpoint.hit_p - point);

		// Berechne Reflexion
		hitpoint.reflection_dir = glm::reflect(ray.direction, hitpoint.normal);

		// Berechne Brechung
		float eta = 1.0f / hitpoint.refraction_index; // Annahme: Das Medium hat einen Brechungsindex
		hitpoint.refraction_dir = glm::refract(ray.direction, hitpoint.normal, eta);

		return hitpoint;
	}
	else {
		return Hitpoint(); // Keine Kollision
	}
	//hinzugefügt ende
}