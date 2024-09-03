#include "box.hpp"
#include "cmath"
#include "ostream"
#include "Hitpoint.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <algorithm>

Box::Box():
	Shape::Shape{"Quader"},
	min_{ 0.0f, 0.0f , 0.0f },
	max_{ 100.0f, 100.0f, 100.0f }
{}

Box::Box(glm::vec3 min, glm::vec3 max) :
	Shape::Shape{"Quader"},
	min_(min),
	max_(max)
{}

Box::Box(glm::vec3 min, glm::vec3 max, std::string name, std::shared_ptr<Material> color) :
	Shape::Shape{name, color},
	min_(min),
	max_(max)
{}

float Box::area()const {
	float a = abs(max_.x - min_.x);
	float b = abs(max_.y - min_.y);
	float c = abs(max_.z - min_.z);
	return 2 * a * b + 2 * a * c + 2 * b * c;
}

float Box::volume()const {
	return abs(max_.x - min_.x) * abs(max_.y - min_.y) * abs(max_.z - min_.z);
}

std::ostream& Box::print(std::ostream& os)const {
	Shape::print(os);
	return os << min_.x << " " << min_.y << " " << min_.z << " " << max_.x << " " << max_.y << " " << max_.z << std::endl;
}

/*
Hitpoint Box::intersect(Ray const& ray)
{
	Hitpoint schnitt = {};
	if (ray.direction.x != 0) {
		float t = (min_.x - ray.origin.x) / ray.direction.x;
		float y = ray.origin.y + (t * ray.direction.y);
		float z = ray.origin.z + (t * ray.direction.z);
		if (min_.y <= y and y <= max_.y and min_.z <= z and z <= max_.z) {
			schnitt.hit = true;
			schnitt.distance = t;
			schnitt.color_obj = get_color();
			schnitt.name_obj = get_name();
			schnitt.hit_p = { min_.x, y, z };
			schnitt.ray_dir = ray.direction;
	}
	}
	else if (ray.direction.x == 0 and ray.direction.y != 0) {
		float t = (min_.y - ray.origin.y) / ray.direction.y;
		float x = ray.origin.x;
		float z = ray.origin.z + (t * ray.direction.z);
		if (min_.x <= x and x <= max_.x and min_.z <= z and z <= max_.z) {
			schnitt.hit = true;
			schnitt.distance = t;
			schnitt.color_obj = get_color();
			schnitt.name_obj = get_name();
			schnitt.hit_p = { x, min_.y, z };
			schnitt.ray_dir = ray.direction;
		}
	}
	else {
		float t = (min_.z - ray.origin.z) / ray.direction.z;
		float x = ray.origin.x;
		float y = ray.origin.y;
		if (min_.x <= x and x <= max_.x and min_.y <= y and y <= max_.y) {
			schnitt.hit = true;
			schnitt.distance = t;
			schnitt.color_obj = get_color();
			schnitt.name_obj = get_name();
			schnitt.hit_p = { x, y, min_.z };
			schnitt.ray_dir = ray.direction;
		}
	}
	
	return schnitt;
}*/

/*Hitpoint Box::intersect(Ray const& ray) {                                    // Deklaration der intersect-Methode für die Box-Klasse, die einen Ray-Parameter akzeptiert und ein HitPoint-Objekt zurückgibt.
	float tmin = (min_.x - ray.origin.x) / ray.direction.x;                        // Berechnung der Eintritts- und Austrittszeiten für die x-Koordinate. tmin ist die Zeit, zu der der Strahl die minimale x-Grenze der Box erreicht.
	float tmax = (max_.x - ray.origin.x) / ray.direction.x;                        // tmax ist die Zeit, zu der der Strahl die maximale x-Grenze der Box erreicht.

	if (tmin > tmax) std::swap(tmin, tmax);                                        // Sicherstellen, dass tmin immer kleiner oder gleich tmax ist. Wenn nicht, werden die Werte vertauscht.

	float tymin = (min_.y - ray.origin.y) / ray.direction.y;                       // Berechnung der Eintritts- und Austrittszeiten für die y-Koordinate. tymin ist die Zeit, zu der der Strahl die minimale y-Grenze der Box erreicht.
	float tymax = (max_.y - ray.origin.y) / ray.direction.y;                       // tymax ist die Zeit, zu der der Strahl die maximale y-Grenze der Box erreicht.

	if (tymin > tymax) std::swap(tymin, tymax);                                    // Sicherstellen, dass tymin immer kleiner oder gleich tymax ist. Wenn nicht, werden die Werte vertauscht.

	if ((tmin > tymax) || (tymin > tmax))                                          // Prüfen, ob die Zeitintervalle für x und y sich überschneiden. Wenn nicht, gibt es keinen Schnittpunkt.
		return Hitpoint();    // Wenn sich die Intervalle nicht überschneiden, gibt es keinen Schnittpunkt. Ein HitPoint-Objekt mit `hit = false` wird zurückgegeben.

	if (tymin > tmin) tmin = tymin;                                                // Aktualisieren von tmin, um den spätesten Eintrittspunkt zwischen x und y zu reflektieren.
	if (tymax < tmax) tmax = tymax;                                                // Aktualisieren von tmax, um den frühesten Austrittspunkt zwischen x und y zu reflektieren.

	float tzmin = (min_.z - ray.origin.z) / ray.direction.z;                       // Berechnung der Eintritts- und Austrittszeiten für die z-Koordinate. tzmin ist die Zeit, zu der der Strahl die minimale z-Grenze der Box erreicht.
	float tzmax = (max_.z - ray.origin.z) / ray.direction.z;                       // tzmax ist die Zeit, zu der der Strahl die maximale z-Grenze der Box erreicht.

	if (tzmin > tzmax) std::swap(tzmin, tzmax);                                    // Sicherstellen, dass tzmin immer kleiner oder gleich tzmax ist. Wenn nicht, werden die Werte vertauscht.

	if ((tmin > tzmax) || (tzmin > tmax))                                          // Prüfen, ob die Zeitintervalle für x, y und z sich überschneiden. Wenn nicht, gibt es keinen Schnittpunkt.
		return Hitpoint();    // Wenn sich die Intervalle nicht überschneiden, gibt es keinen Schnittpunkt. Ein HitPoint-Objekt mit `hit = false` wird zurückgegeben.

	if (tzmin > tmin) tmin = tzmin;                                                // Aktualisieren von tmin, um den spätesten Eintrittspunkt zwischen x, y und z zu reflektieren.
	if (tzmax < tmax) tmax = tzmax;                                                // Aktualisieren von tmax, um den frühesten Austrittspunkt zwischen x, y und z zu reflektieren.

	if (tmin < 0) {                                                                // Wenn tmin negativ ist, liegt der Schnittpunkt hinter dem Ursprung des Strahls.
		tmin = tmax;                                                               // Setzen von tmin auf tmax, um den nächsten möglichen Schnittpunkt zu finden.
		if (tmin < 0) return Hitpoint(); // Wenn tmin immer noch negativ ist, gibt es keinen Schnittpunkt. Ein HitPoint-Objekt mit `hit = false` wird zurückgegeben.
	}

	glm::vec3 intersection_point = ray.origin + tmin * ray.direction;              // Berechnen des Schnittpunktes in Weltkoordinaten, indem die Richtung des Strahls mit tmin multipliziert und zum Ursprung des Strahls addiert wird.
	return Hitpoint(true, tmin, name_, color_, intersection_point, ray.direction); // Rückgabe eines HitPoint-Objekts mit den Informationen des Schnittpunkts, einschließlich `hit = true`.

}*/

Hitpoint Box::intersect(Ray const& ray) {
	// Berechne die Entfernung entlang der x-Achse, bei der der Strahl die Box zuerst und zuletzt trifft
	float t_near_x = (min_.x - ray.origin.x) / ray.direction.x;
	float t_far_x = (max_.x - ray.origin.x) / ray.direction.x;
	if (t_near_x > t_far_x) std::swap(t_near_x, t_far_x);

	// Berechne die Entfernung entlang der y-Achse, bei der der Strahl die Box zuerst und zuletzt trifft
	float t_near_y = (min_.y - ray.origin.y) / ray.direction.y;
	float t_far_y = (max_.y - ray.origin.y) / ray.direction.y;
	if (t_near_y > t_far_y) std::swap(t_near_y, t_far_y);

	// Überprüfe, ob der Strahl überhaupt die Box treffen kann, indem man die Bereiche entlang der x- und y-Achse vergleicht
	if ((t_near_x > t_far_y) || (t_near_y > t_far_x)) {
		return Hitpoint(); // Kein Schnittpunkt
	}

	// Aktualisiere den nächsten Schnittpunkt, um sicherzustellen, dass wir den spätesten Eintrittspunkt haben
	if (t_near_y > t_near_x) t_near_x = t_near_y;
	if (t_far_y < t_far_x) t_far_x = t_far_y;

	// Berechne die Entfernung entlang der z-Achse, bei der der Strahl die Box zuerst und zuletzt trifft
	float t_near_z = (min_.z - ray.origin.z) / ray.direction.z;
	float t_far_z = (max_.z - ray.origin.z) / ray.direction.z;
	if (t_near_z > t_far_z) std::swap(t_near_z, t_far_z);

	// Überprüfe erneut, ob der Strahl die Box treffen kann, diesmal unter Berücksichtigung der z-Achse
	if ((t_near_x > t_far_z) || (t_near_z > t_far_x)) {
		return Hitpoint(); // Kein Schnittpunkt
	}

	// Aktualisiere den nächsten Schnittpunkt basierend auf der z-Achse
	if (t_near_z > t_near_x) t_near_x = t_near_z;
	if (t_far_z < t_far_x) t_far_x = t_far_z;

	// Wenn der Schnittpunkt vor dem Ursprung des Strahls liegt, gibt es keinen gültigen Schnittpunkt
	if (t_near_x < 0) {
		t_near_x = t_far_x;
		if (t_near_x < 0) {
			return Hitpoint(); // Kein Schnittpunkt
		}
	}

	// Berechne den Schnittpunkt und erstelle das Hitpoint-Objekt
	glm::vec3 intersection_point = ray.origin + t_near_x * ray.direction;
	return Hitpoint(true, t_near_x, name_, color_, intersection_point, ray.direction);
}
