// ray.cpp
#include "ray.hpp"                 // Inkludiert die Header-Datei für die Ray-Struktur
#include <glm/gtc/matrix_transform.hpp> // Inkludiert GLM-Funktionen für Matrixtransformationen

Ray transformRay(glm::mat4 const& mat, Ray const& ray) {
    // Diese Funktion transformiert einen Strahl mit einer 4x4-Matrix

    Ray transformedRay;            // Erstellt einen neuen Strahl für das Ergebnis

    // Transformiert den Ursprung des Strahls
    // Die 1.0f am Ende macht es zu einem Punkt im homogenen Koordinatensystem
    transformedRay.origin = glm::vec3(mat * glm::vec4(ray.origin, 1.0f));

    // Transformiert die Richtung des Strahls
    // Die 0.0f am Ende macht es zu einem Vektor im homogenen Koordinatensystem
    transformedRay.direction = glm::vec3(mat * glm::vec4(ray.direction, 0.0f));

    return transformedRay;         // Gibt den transformierten Strahl zurück
}