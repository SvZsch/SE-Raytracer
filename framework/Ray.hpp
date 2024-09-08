#ifndef RAY_HPP                    // Verhindert mehrfaches Einbinden dieser Datei
#define RAY_HPP

#include <glm/vec3.hpp>            // Bindet die GLM-Bibliothek f�r 3D-Vektoren ein

struct Ray {                       // Definiert eine Struktur f�r einen Strahl
    glm::vec3 origin = { 0.0f, 0.0f, 0.0f };    // Ursprungspunkt des Strahls, standardm��ig im Ursprung (0,0,0)
    glm::vec3 direction = { 0.0f, 0.0f, -1.0f }; // Richtungsvektor des Strahls, standardm��ig in negative Z-Richtung
};

#endif                             // Beendet die Schutz vor Mehrfacheinbindung