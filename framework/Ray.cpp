// ray.cpp
#include "ray.hpp"
#include <glm/gtc/matrix_transform.hpp>

Ray transformRay(glm::mat4 const& mat, Ray const& ray) {
    Ray transformedRay;
    transformedRay.origin = glm::vec3(mat * glm::vec4(ray.origin, 1.0f));
    transformedRay.direction = glm::vec3(mat * glm::vec4(ray.direction, 0.0f));
    return transformedRay;
}