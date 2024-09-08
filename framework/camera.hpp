// camera.hpp
#ifndef CAMERA_HPP                 // Verhindert mehrfaches Einbinden dieser Datei
#define CAMERA_HPP

#include <glm/glm.hpp>             // Bindet die GLM-Bibliothek für mathematische Operationen ein
#include <string>                  // Bindet die String-Klasse ein

struct Camera {                    // Definiert eine Struktur für eine Kamera
    std::string name;              // Name der Kamera
    float fov_x;                   // Horizontales Sichtfeld (Field of View) in Grad
    glm::vec3 eye;                 // Position der Kamera im 3D-Raum
    glm::vec3 dir;                 // Blickrichtung der Kamera
    glm::vec3 up;                  // Aufwärtsvektor der Kamera

    Camera(const std::string& n, float fov, const glm::vec3& e, const glm::vec3& d, const glm::vec3& u)
        : name(n), fov_x(fov), eye(e), dir(d), up(u) {} // Konstruktor zur Initialisierung aller Kamera-Eigenschaften
};

#endif // CAMERA_HPP               // Beendet die Schutz vor Mehrfacheinbindung