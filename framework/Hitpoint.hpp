#ifndef HITPOINT_HPP               // Verhindert mehrfaches Einbinden dieser Datei
#define HITPOINT_HPP

#include <glm/vec3.hpp>            // Für 3D-Vektoren
#include <string>                  // Für string-Objekte
#include "Material.hpp"            // Bindet Material-Definitionen ein

struct Hitpoint {                  // Definiert eine Struktur für Trefferpunkte
    bool hit{ false };             // Gibt an, ob ein Treffer stattgefunden hat
    float distance{ 0.0f };        // Entfernung zum Trefferpunkt
    float t = 0.0f;                // Parameter für den Strahl beim Treffer
    std::string name_obj{ "none" }; // Name des getroffenen Objekts
    std::shared_ptr<Material> color_obj; // Material/Farbe des getroffenen Objekts
    glm::vec3 hit_p{ 0.0f, 0.0f, 0.0f }; // Koordinaten des Trefferpunkts
    glm::vec3 ray_dir{ 0.0f, 0.0f, 0.0f }; // Richtung des auftreffenden Strahls

    glm::vec3 normal{ 0.0f, 0.0f, 0.0f };  // Normalenvektor an der Trefferoberfläche
    glm::vec3 reflection_dir{ 0.0f, 0.0f, 0.0f };  // Richtung des reflektierten Strahls
    glm::vec3 refraction_dir{ 0.0f, 0.0f, 0.0f }; // Richtung des gebrochenen Strahls
    float refraction_index{ 1.0f }; // Brechungsindex des Materials am Trefferpunkt
};

#endif                             // Beendet die Schutz vor Mehrfacheinbindung