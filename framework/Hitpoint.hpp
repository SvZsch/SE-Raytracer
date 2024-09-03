#ifndef HITPOINT_HPP
#define HITPOINT_HPP
#include <glm/vec3.hpp>
#include <string>
#include "Material.hpp"
struct Hitpoint {
	bool hit{ false };
	float distance{ 0.0f };
	std::string name_obj{ "none" };
	std::shared_ptr<Material> color_obj;
	glm::vec3 hit_p{ 0.0f, 0.0f, 0.0f };
	glm::vec3 ray_dir{ 0.0f, 0.0f, 0.0f };

	//hinzugefügt
	glm::vec3 normal{ 0.0f, 0.0f, 0.0f };  // Neuer Normalenvektor
	glm::vec3 reflection_dir{ 0.0f, 0.0f, 0.0f };  // Neue Richtung für Spiegelung
	glm::vec3 refraction_dir{ 0.0f, 0.0f, 0.0f }; // Neue Richtung für Brechung
	float refraction_index{ 1.0f }; // Brechungsindex des Materials
	//hinzugefügt ende
};
#endif