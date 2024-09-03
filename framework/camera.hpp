// camera.hpp
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <string>

struct Camera {
    std::string name;
    float fov_x;
    glm::vec3 eye;
    glm::vec3 dir;
    glm::vec3 up;

    Camera(const std::string& n, float fov, const glm::vec3& e, const glm::vec3& d, const glm::vec3& u)
        : name(n), fov_x(fov), eye(e), dir(d), up(u) {}
};

#endif // CAMERA_HPP